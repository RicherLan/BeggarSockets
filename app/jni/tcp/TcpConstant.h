/** 
 * author: BeggarLan
 * created on: 2023/7/5 10:01 
 * description: 放一些常量、枚举等，大杂烩，非常不合理
 */

#ifndef BEGGARSOCKETS_TCPCONSTANT_H
#define BEGGARSOCKETS_TCPCONSTANT_H

#include <stdint.h>
#include <string.h>
#include <functional>

class NativeByteBuffer

/**
 * 所有的event
 */
enum EventType {
    Connection, // 连接
    Pipe,        // 管道
    Fd,         // EventFd
    Timer,      // 定时器
};

/**
 * 请求完成回调
 */
typedef std::function<void(NativeByteBuffer *buffer, uint32_t code, string desc)> onSendCompleteFunc

// 请求的flag
enum RequestFlag {
    WithoutAck = 1,  // 没有回包
    ReSend = 2,      // 重发
    WithoutLogin = 3 // 可以在握手前发送
};

#endif //BEGGARSOCKETS_TCPCONSTANT_H
