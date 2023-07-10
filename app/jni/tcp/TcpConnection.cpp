/** 
 * author: BeggarLan
 * created on: 2023/7/7 15:21 
 * description: 
 */

#include "TcpConnection.h"
#include "DataCenter.h"
#include "NativeByteBuffer.h"
#include "BufferStorage.h"
#include "ConnectionManager.h"


TcpConnection::TcpConnection(const DataCenter &dataCenter) {
    this.dataCenter = dataCenter;
    connectionStatus = TcpConnectionStatus::Idle;
    // [&] 匿名函数，可以引用外部变量
    reconnectTimer = new Timer([&] {
        reconnectTimer->stop();
        // 超时重连
        connect();
    })
}

TcpConnection::~TcpConnection() {
    if (reconnectTimer != nullptr) {
        reconnectTimer->stop();
        delete reconnectTimer;
        reconnectTimer = nullptr;
    }
}

void TcpConnection::connect() {
    // todo 检查网路

    // 连接中 || 已经连接
    if (connectionStatus == TcpConnectionStatus::Connecting ||
        connectionStatus == TcpConnectionStatus::Connected) {
        return;
    }
    // 更新状态为连接中
    connectionStatus = TcpConnectionStatus::Connecting;
    // 停止重连
    reconnectTimer->stop();
    // 清除上一个连接的数据
    if(notFullData!= nullptr) {
        notFullData->reuse();
        notFullData = nullptr;
    }
    // 通知外面连接中
    // todo 依赖反了，应该注册回调
    ConnectionManager::getInstance().onConnectStatusChanged(connectionStatus);

    serverAddress = dataCenter.getCurrentAddress();
    serverPort = dataCenter.getCurrentPort();
    openConnection(serverAddress, serverPort);

    // todo 设置超时
}

void TcpConnection::reconnect() {

}

void TcpConnection::closeConnect() {

}

void TcpConnection::sendData(NativeByteBuffer *buffer) {

}

void TcpConnection::onReceiveData(NativeByteBuffer *buffer) {

}

void TcpConnection::onConnected() {

}

void TcpConnection::onDisconnected(int reason) {

}
