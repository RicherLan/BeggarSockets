/** 
 * author: BeggarLan
 * created on: 2023/7/6 11:14 
 * description: tcpSocket
 */

#ifndef BEGGARSOCKETS_TCPSOCKET_H
#define BEGGARSOCKETS_TCPSOCKET_H

#include <sys/epoll.h>
#include "string"
#include <netinet/in.h>

class ByteStream;
class NativeByteBuffer;
class EventsDispatcher;
class SocketCloseReason;

class TcpSocket {
private:
    // address
    struct sockaddr_in socketAddress;
    struct sockaddr_in6 socketAddress6;
    // write字节流
    ByteStream *byteStream = nullptr;

    // 当前的socket
    int sockedFd = -1;
    // epoll_event
    struct epoll_event socketEvent;
    // 事件分发
    EventsDispatcher *eventsDispatcher = nullptr;

    time_t timeoutMs;
    // 上次事件的时间
    int64_t  lastEventTimeMs;

    // 连接是否已经建立：注意tcp一连接上就会产生EPOLLOUT事件
    bool connected = false;

public:
    TcpSocket();
    ~TcpSocket();

    // 连接
    void openConnection(std::string address, uint16_t port);
    void dropConnection();
    bool isConnected();
    // 写数据
    void writeBuffer(NativeByteBuffer *buffer);
    // 设置超时时间
    void setTimeout(time_t timeMs)

    // todo
protected:
    void onEvent(uint32_t events);
    bool checkTimeout(int64_t currentTimeMs);
    void setLastEventTime(int64_t eventTimeMs);
    int64_t getLastEventTimeMs();

    virtual void onReceiveData(NativeByteBuffer *buffer) = 0;
    virtual void onConnected() = 0;
    virtual void onDisconnected(int reason) {};

private:
    void closeSocket(SocketCloseReason reason);
    bool checkSocketError();
    // 连接
    void openConnectionIpv4(std::string address, uint16_t port);
    void openConnectionIpv6(std::string address, uint16_t port);
    // 通知写操作
    void notifyWriteOp();
};

#endif //BEGGARSOCKETS_TCPSOCKET_H
