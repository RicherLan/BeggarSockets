/** 
 * author: BeggarLan
 * created on: 2023/7/6 15:19 
 * description: tcp客户端
 */

#ifndef BEGGARSOCKETS_TCPCONNECTION_H
#define BEGGARSOCKETS_TCPCONNECTION_H

#include "TcpSocket.h"
#include "TcpConnectionStatus.h"

class DataCenter;

// todo 不应该继承，应该是持有关系
class TcpConnection: public TcpSocket {
public:
    TcpConnection();
    ~TcpConnection();

    void connect();
    void reconnect();
    void closeConnect();

    void sendData(NativeByteBuffer *buffer);

private:
    // server的地址
    std::string serverAddress;
    uint16_t serverPort;
    // 获取数据用
    DataCenter& dataCenter;
    // 连接状态
    TcpConnectionStatus connectionStatus = TcpConnectionStatus::Idle;
};

#endif //BEGGARSOCKETS_TCPCONNECTION_H
