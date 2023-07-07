/** 
 * author: BeggarLan
 * created on: 2023/7/7 11:36 
 * description: 
 */

#include "sys/socket.h"
#include "arpa/inet.h"
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
    sockedFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockedFd < 0) {

    }

}

// 连接ipv6
void TcpSocket::openConnectionIpv6(std::string address, uint16_t port) {

}

void TcpSocket::dropConnection() {

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

}

bool TcpSocket::checkTimeout(int64_t currentTimeMs) {

}

void TcpSocket::setLastEventTime(int64_t eventTimeMs) {

}

int64_t TcpSocket::getLastEventTimeMs() {

}

void TcpSocket::closeSocket(SocketCloseReason reason) {

}

bool TcpSocket::checkSocketError() {

}

// bufferStream有改变时调用
void TcpSocket::adjustWriteOp() {

}