/** 
 * author: BeggarLan
 * created on: 2023/7/6 17:38 
 * description: 整个tcp客户端的接口层: 握手、发消息、收消息等
 */

#ifndef BEGGARSOCKETS_CONNECTIONMANAGER_H
#define BEGGARSOCKETS_CONNECTIONMANAGER_H

#include "stdint.h"
#include "string"
#include <sys/epoll.h>
#include "TcpConnectionStatus.h"

class EventsDispatcher;
class TcpSocket;
class TcpConnection;
class NativeByteBuffer;
class ConnectionManagerCallback;

// todo 目前一个ConnectionManager只对应一个connection
class ConnectionManager {
public:
    ~ConnectionManager();
    static ConnectionsManager &getInstance();

    // 初始化
    void init();
    // 添加server的连接地址
    void addServerAddressPort(std::string address ,uint16_t port);
    // 设置回调
    void setCallback(ConnectionManagerCallback *callback)
    // 发消息
    void send(NativeByteBuffer *buffer, int32_t cmdId, uint32_t flags, uint64_t timeoutMs, onSendCompleteFunc sendCompleteFunc);

    // 清理
    void clear();



private:
    // single instance
    ConnectionManager();
    // 调度一个task
    void scheduleTask(std::function<void()> task);

    // socket连接的时候调用
    void attachConnection(TcpSocket *connection);
    // socket关闭的时候调用
    void detachConnection(TcpSocket *connection);
    // 连接状态改变
    void onConnectStatusChanged(TcpConnectionStatus status);


    // todo 放在这里是不合理的
    // 负责整个tcp的epoll
    int epollFd;

    // 读写数据时的buffer
    NativeByteBuffer *networkBuffer;


    // todo 依赖关系反了
    friend class TcpSocket;
    friend class TcpConnection;
};

#endif //BEGGARSOCKETS_CONNECTIONMANAGER_H
