/** 
 * author: BeggarLan
 * created on: 2023/7/6 15:47 
 * description: tcp连接状态
 */

#ifndef BEGGARSOCKETS_TCPCONNECTIONSTATUS_H
#define BEGGARSOCKETS_TCPCONNECTIONSTATUS_H

enum TcpConnectionStatus {
    Idle,
    Connecting,
    Reconnecting,
    Connected,
    closed
};

#endif //BEGGARSOCKETS_TCPCONNECTIONSTATUS_H
