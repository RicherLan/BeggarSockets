/** 
 * author: BeggarLan
 * created on: 2023/7/6 17:38 
 * description: 整个tcp客户端的接口层: 握手、发消息、收消息等
 */

#ifndef BEGGARSOCKETS_CONNECTITONMANAGER_H
#define BEGGARSOCKETS_CONNECTITONMANAGER_H

#include "stdint.h"
#include "string"

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
    ConnectionManager();
    // 调度一个task
    void scheduleTask(std::function<void()> task);
};

#endif //BEGGARSOCKETS_CONNECTITONMANAGER_H
