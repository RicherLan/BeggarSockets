/** 
 * author: BeggarLan
 * created on: 2023/7/6 18:29 
 * description: 回调出ConnectionManager的关键事件
 */

#ifndef BEGGARSOCKETS_CONNECTIONMANAGERCALLBACK_H
#define BEGGARSOCKETS_CONNECTIONMANAGERCALLBACK_H

#include "stdint.h"
#include "TcpConnectionStatus.h"
#include "NativeByteBuffer.h"

class ConnectionManagerCallback {
    // 连接状态改变
    virtual void onConnectionStateChanged(TcpConnectionStatus status) = 0;

    // 收到消息
    virtual void onReceiveMessage(int32_t cmd, NativeByteBuffer *buffer) = 0;

    // 开始握手
    virtual void onHandshakeBegin() = 0;

    // 握手结束
    virtual void onHandshakeResult(NativeByteBuffer *buffer) = 0;
};

#endif //BEGGARSOCKETS_CONNECTIONMANAGERCALLBACK_H
