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

// 默认的包超时时间
#define DEFAULT_PACK_TIMEOUT 30 * 1000
// 默认的重试次数
#define DEFAULT_RETRY_COUNT 3

class NativeByteBuffer

/**
 * 所有的event
 */
enum EventType {
    EventConnection, // 连接
    EventPipe,        // 管道
    EventFd,         // EventFd
    EventTimer,      // 定时器
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
