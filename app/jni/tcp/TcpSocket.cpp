/** 
 * author: BeggarLan
 * created on: 2023/7/7 11:36 
 * description: 
 */

#include "sys/socket.h"
#include "arpa/inet.h"
#include <errno.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include "string"
#include "TcpSocket.h"
#include "NativeByteBuffer.h"
#include "ByteStream.h"
#include "EventsDispatcher.h"
#include "ConnectionManager.h"
#include "TimeUtil.h"
#include "SocketCloseReason.h"


TcpSocket::TcpSocket() {
    eventsDispatcher = new EventsDispatcher(this, EventConnection);
    byteStream = new ByteStream();
    lastEventTimeMs = getCurrentTimeMonotonicMillis();
}

TcpSocket::~TcpSocket() {
    if (eventsDispatcher != nullptr) {
        delete eventsDispatcher;
        eventsDispatcher = nullptr;
    }
    if (byteStream != nullptr) {
        delete byteStream;
        byteStream = nullptr;
    }
}

// 连接
void TcpSocket::openConnection(std::string address, uint16_t port) {
    // 先清空
    memset(&socketAddress, 0, sizeof(socketAddress));

    // inet_pton是一个IP地址转换函数，可以在将IP地址在“点分十进制”和“二进制整数”之间转换
    // 第一个参数af是地址簇，第二个参数*src是来源地址，第三个参数* dst接收转换后的数据

    // ipv4
    if (inet_pton(AF_INET, address.c_str(), &socketAddress.sin_addr.s_addr) == 1) {
        openConnectionIpv4(address, port);
        return;
    }
    // ipv6
    if (inet_pton(AF_INET6, address.c_str(), &socketAddress.sin_addr.s_addr) == 1) {
        openConnectionIpv6(address, port);
        return;
    }
}

// 连接ipv4
void TcpSocket::openConnectionIpv4(std::string address, uint16_t port) {
    int epollFd = ConnectionManager::getInstance().epollFd;
    // 创建socket
    sockedFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockedFd < 0) {
        closeSocket(SocketCloseReason::COMMON)
        return;
    }
    socketAddress.sin_port = htons(port);
    socketAddress.sin_family = AF_INET;

    // 设置tcp noDelay
    int ok;
    setsockopt(sockedFd, IPPROTO_TCP, TCP_NODELAY, &ok, sizeof(int))

    // 设置socket为非堵塞
    if (fcntl(sockedFd, F_SETFL, O_NONBLOCK) == -1) {
        closeSocket(SocketCloseReason::COMMON);
        return;
    }

    /**
     * 非阻塞connect返回的错误是有讲究的
     * 1. 如果非阻塞connect返回的错误是EINPROGRESS，代表不是connect系统调用出错了，而是connect可能会在后面才会建立完整地连接(只是当前连接还没有建立完整)，所以我们可以在通过给select、pol或epoll设置等待时间，来等待这个connect的连接成功，从而进一步处理
     * 2. 如果非阻塞connect返回的错误不是EINPROGRESS，代表就是connect系统调用本身出错了，那么就可以做一些相应的错误处理了
     */
    int result = connect(sockedFd, (sockaddr * ) & socketAddress, (socklen_t)
    sizeof(sockaddr_in));
    if (result == -1 && errno != EINPROGRESS) {
        closeSocket(SocketCloseReason::COMMON)
    } else {
        ConnectionManager::getInstance().attachConnection(this);
        /**
         * EPOLLRDHUP：读关闭
         * 对端正常断开连接（调用close())，在服务器端会触发一个epoll事件。在低于2.6.17版本的内核中，这个epoll事件一般是EPOLLIN，即0x1表示连接可读。
         * 连接池检测到某个连接发生EPOLLIN事件且没有错误后，会认为有请求到来，将连接交给上层进行处理。这样以来，上层尝试在对端已经close()的连接上读取请求，只能读到EOF，会认为发生异常，报告一个错误。
         * 连接池检测到某个连接发生EPOLLIN事件且没有错误后，会认为有请求到来，将连接交给上层进行处理。这样以来，上层尝试在对端已经close()的连接上读取请求，只能读到EOF，会认为发生异常，报告一个错误。
         * 因此在使用2.6.17之前版本内核的系统中，我们无法依赖封装epoll的底层连接库来实现对对端关闭连接事件的检测，只能通过上层读取数据进行区分处理。
         * 因此在使用2.6.7版本内核中增加EPOLLRDHUP事件，表示对端断开连接，关于添加这个事件的理由可以参见 http://lkml.org/lkml/2003/7/12/116。
         */
        /**
         * Level-triggered ：水平触发，缺省模式
         * edge-triggered ：边缘触发
         * 比如redis用LT模式，nginx用ET模式
         *
         * 通知模式：
         * LT模式时，事件就绪时，假设对事件没做处理，内核会反复通知事件就绪
         * ET模式时，事件就绪时，假设对事件没做处理，内核不会反复通知事件就绪
         *
         * 事件通知的细节：
         * 1.调用epoll_ctl，ADD或者MOD事件EPOLLIN
         *     LT：如果此时缓存区没有可读数据，则epoll_wait不会返回EPOLLIN，如果此时缓冲区有可读数据，则epoll_wait会持续返回EPOLLIN
         *     ET：如果此时缓存区没有可读数据，则epoll_wait不会返回EPOLLIN，如果此时缓冲区有可读数据，则epoll_wait会返回一次EPOLLIN
         * 2.调用epoll_ctl，ADD或者MOD事件EPOLLOUT
         *     LT：如果不调用epoll_ctl将EPOLLOUT修改为EPOLLIN，则epoll_wait会持续返回EPOLLOUT(前提条件是写缓冲区未满)
         *     ET：epoll_wait只会返回一次EPOLLOUT
         */
        // 监听可写、可读、对端断开、error、ET
        socketEvent.events = EPOLLOUT | EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLET;
        // todo 这里应该传自定义callback，而不是让EventsDispatcher集中处理
        socketEvent.data.ptr = eventsDispatcher;
        // 添加到epoll中
        int code = epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &socketEvent);
        if (code != 0) {
            closeSocket(SocketCloseReason::COMMON)
        }
    }
}

// 连接ipv6
// todo copy自openConnectionIpv4
void TcpSocket::openConnectionIpv6(std::string address, uint16_t port) {
    int epollFd = ConnectionManager::getInstance().epollFd;
    sockedFd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockedFd < 0) {
        closeSocket(SocketCloseReason::COMMON)
        return;
    }
    socketAddress6.sin6_port = htons(port);
    socketAddress6.sin6_family = AF_INET6;

    int ok;
    setsockopt(sockedFd, IPPROTO_TCP, TCP_NODELAY, &ok, sizeof(int))

    if (fcntl(sockedFd, F_SETFL, O_NONBLOCK) == -1) {
        closeSocket(SocketCloseReason::COMMON);
        return;
    }

    int result = connect(sockedFd, (sockaddr * ) & socketAddress6, (socklen_t)
    sizeof(socketAddress6));
    if (result == -1 && errno != EINPROGRESS) {
        closeSocket(SocketCloseReason::COMMON)
    } else {
        ConnectionManager::getInstance().attachConnection(this);
        socketEvent.events = EPOLLOUT | EPOLLIN | EPOLLRDHUP | EPOLLERR | EPOLLET;
        socketEvent.data.ptr = eventsDispatcher;
        int code = epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &socketEvent);
        if (code != 0) {
            closeSocket(SocketCloseReason::COMMON)
        }
    }
}

void TcpSocket::dropConnection() {
    closeSocket(SocketCloseReason::MANUAL)
}

bool TcpSocket::isConnected() {

}

// 写数据
void TcpSocket::writeBuffer() {

}

// 设置超时时间
void TcpSocket::setTimeout(time_t timeMs) {

}

void TcpSocket::onEvent(uint32_t events) {
    // 对端关闭时，自己会收到EPOLLRDHUP 和 EPOLLIN(有读事件)，因此要先处理
    if ((events & EPOLLRDHUP) || (events & EPOLLHUP)) {
        closeSocket(SocketCloseReason::COMMON);
        return;
    }
    if (events & EPOLLERR) {
        // todo
        return;
    }

    // 可读事件
    if (events & EPOLLIN) {
        // 检查错误，比如断网时会收到可读事件
        if (checkSocketError()) {
            closeSocket(SocketCloseReason::COMMON);
            return;
        } else {
            // todo
            NativeByteBuffer *receiveBuffer = ConnectionManager::getInstance().networkBuffer;
            size_t readCount = 0;
            // 循环读
            while (true) {
                readCount = recv(sockedFd, receiveBuffer->bytes(), receiveBuffer->capacity(), 0);
                if (readCount < 0) {
                    closeSocket(SocketCloseReason::MANUAL);
                    return;
                }
                // 读到数据
                if (readCount > 0) {
                    receiveBuffer->rewind();
                    receiveBuffer->limit(readCount);
                    onReceiveData(receiveBuffer);
                }
                // 读完了
                if (readCount != receiveBuffer->capacity()) {
                    break;
                }
            }
        }
    }

    // 可写事件
    if (events & EPOLLOUT) {
        // 检查错误
        if (checkSocketError()) {
            closeSocket(SocketCloseReason::COMMON);
            return;
        } else {
            // tcp一连接上就会产生EPOLLOUT事件, 这里第一次收到可写时作为onConnected()
            if (!connected) {
                connected = true;
                onConnected();
            }
        }
    }
}

bool TcpSocket::checkTimeout(int64_t currentTimeMs) {

}

void TcpSocket::setLastEventTime(int64_t eventTimeMs) {

}

int64_t TcpSocket::getLastEventTimeMs() {

}

void TcpSocket::closeSocket(SocketCloseReason reason) {
    ConnectionManager::getInstance().detachConnection(this);

    int epollFd = ConnectionManager::getInstance().epollFd;
    if (sockedFd >= 0) {
        // 从epoll中移除
        epoll_ctl(epollFd, EPOLL_CTL_DEL, socketFd, NULL);
        if (close(sockedFd) != 0) {
            // can't close socket
        }
        sockedFd = -1;
    }

    // 更新连接状态
    connected = false;

    // 清除buffer
    if (byteStream != nullptr) {
        byteStream->clear();
    }

    // 回调连接断开
    onDisconnected(reason);
}

bool TcpSocket::checkSocketError() {
    if (sockedFd < 0) {
        return true;
    }
    int code;
    int opt = getsockopt(sockedFd, SOL_SOCKET, SO_ERROR, code, sizeof(int));
    return code != 0 || opt != 0;
}

// bufferStream有改变时调用
void TcpSocket::adjustWriteOp() {

}