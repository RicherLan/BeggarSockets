/** 
 * author: BeggarLan
 * created on: 2023/7/5 10:42 
 * description: 请求
 */

#ifndef BEGGARSOCKETS_REQUEST_H
#define BEGGARSOCKETS_REQUEST_H

#include "TcpConstant.h"

class NativeByteBuffer;

class Request {
public:
    Request(NativeByteBuffer *buffer, int32_t cmdId, uint32_t flags, uint64_t timeoutMs, onSendCompleteFunc sendCompleteFunc);
    ~Request();

    // ***************** 必要的参数
    // 请求的buffer
    NativeByteBuffer *buffer;
    // 命令
    int32_t cmdId;
    // 一些flag
    uint32_t flags;
    // 超时时间
    uint64_t timeoutMs;
    // 请求完成的回调
    onSendCompleteFunc  sendCompleteFunc;

    // ***************** other
    // 请求开始的时候更新(包括retry)
    uint64_t startTimeMs;
    // 重试次数
    int32_t retryCount;
    // token
    uint32_t connectionToken = 0;
    // 标识消息的
    int32_t  messageSeq = 0;

    void onComplete(NativeByteBuffer *buffer, uint32_t code, string desc);
    bool isTimeout(uint64_t currentTimeMs);
    bool canRetry();
};

#endif //BEGGARSOCKETS_REQUEST_H
