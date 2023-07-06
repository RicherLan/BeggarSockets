/** 
 * author: BeggarLan
 * created on: 2023/7/6 15:19 
 * description: tcp客户端
 */

#ifndef BEGGARSOCKETS_TCPCONNECTION_H
#define BEGGARSOCKETS_TCPCONNECTION_H

#include "TcpSocket.h"

// todo 不应该继承，应该是持有关系
class TcpConnection: public TcpSocket {
public:
    TcpConnection();
    ~TcpConnection();

    void connect();
    void reconnect();
    void closeConnect();

    void sendData(NativeByteBuffer *buffer);
};

#endif //BEGGARSOCKETS_TCPCONNECTION_H
