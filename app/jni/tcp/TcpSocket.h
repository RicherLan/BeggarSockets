/** 
 * author: BeggarLan
 * created on: 2023/7/6 11:14 
 * description: 封装tcpSocket
 */

#ifndef BEGGARSOCKETS_TCPSOCKET_H
#define BEGGARSOCKETS_TCPSOCKET_H

#include <sys/epoll.h>
#include "string"
#include <netinet/in.h>

class ByteStream;
class NativeByteBuffer;
class EventsDispatcher;

class TcpSocket {
private:
    // address
    struct sockaddr_in socketAddress;
    struct sockaddr_in6 socketAddress6;
    // 字节流
    ByteStream *byteStream = nullptr;

    // eventFd，可以通过该字段判断连接是否建立
    int sockedFd = -1;
    // epoll_event
    struct epoll_event event;
    // 事件分发
    EventsDispatcher *eventsDispatcher = nullptr;

    time_t timeoutMs;
    int64_t  lastEventTimeMs;

public:
    TcpSocket();
    ~TcpSocket();

    // 连接
    void openConnection(std::string address, uint16_t port);
    void dropConnection();
    bool isConnected();
    // 写数据
    void writeBuffer();
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
    void closeSocket(int reason);
    bool checkSocketError();
    // bufferStream有改变时调用
    void adjustWriteOp();
    // 连接
    void openConnectionIpv4(std::string address, uint16_t port);
    void openConnectionIpv6(std::string address, uint16_t port);
};

#endif //BEGGARSOCKETS_TCPSOCKET_H
