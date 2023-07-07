/** 
 * author: BeggarLan
 * created on: 2023/7/6 15:19 
 * description: 封装连接相关的
 */

#ifndef BEGGARSOCKETS_TCPCONNECTION_H
#define BEGGARSOCKETS_TCPCONNECTION_H

#include "TcpSocket.h"
#include "TcpConnectionStatus.h"
#include "PacketHeader.h"

class DataCenter;

// todo 不应该继承，应该是持有关系
class TcpConnection: public TcpSocket {
public:
    // todo DataCenter应该由connection自己管理
    TcpConnection(const DataCenter& dataCenter);
    ~TcpConnection();

    void connect();
    void reconnect();
    void closeConnect();

    void sendData(NativeByteBuffer *buffer);

protected:
    virtual void onReceiveData(NativeByteBuffer *buffer) override;
    virtual void onConnected() override;
    virtual void onDisconnected(int reason) override;

private:
    // server的地址
    std::string serverAddress;
    uint16_t serverPort;
    // 获取数据用
    DataCenter& dataCenter;
    // 连接状态
    TcpConnectionStatus connectionStatus = TcpConnectionStatus::Idle;

    // 检查header是否有问题
    bool checkHeader(PacketHeader& packetHeader);
};

#endif //BEGGARSOCKETS_TCPCONNECTION_H
