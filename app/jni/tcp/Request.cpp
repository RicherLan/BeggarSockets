/** 
 * author: BeggarLan
 * created on: 2023/7/5 15:05 
 * description: 请求
 */


#include "Request.h"
#include "NativeByteBuffer.h"

Request::Request(
        NativeByteBuffer *buffer,
        int32_t cmdId,
        uint32_t flags,
        uint64_t timeoutMs,
        onSendCompleteFunc sendCompleteFunc) {
    this->buffer = buffer;
    this->cmdId = cmdId;
    this->flags = flags;
    this->sendCompleteFunc = std::move(sendCompleteFunc);
    if (timeoutMs > 0) {
        this->timeoutMs = timeoutMs;
    } else {
        timeoutMs = DEFAULT_PACK_TIMEOUT;
    }
}

Request::~Request() {
    if (buffer != nullptr) {
        // todo
    }
}

void Request::onComplete(NativeByteBuffer *buffer, uint32_t code, std::string desc) {
    if (sendCompleteFunc != nullptr) {
        sendCompleteFunc(buffer, code, desc);
    }
}

bool Request::isTimeout(uint64_t currentTimeMs) {
    return currentTimeMs - startTimeMs >= timeoutMs
}

bool Request::canRetry() {
    return retryCount <= DEFAULT_RETRY_COUNT;
}