/** 
 * author: BeggarLan
 * created on: 2023/7/5 16:29 
 * description: 
 */


#include "ByteStream.h"
#include "NativeByteBuffer.h"

ByteStream::ByteStream() {
    bufferQueue = new std::vector<NativeByteBuffer *>;
}

ByteStream::ByteStream(const ByteStream &bt) {
    bufferQueue = new std::vector<NativeByteBuffer *>(*bt.bufferQueue);
}

ByteStream::~ByteStream() {
    delete bufferQueue;
    bufferQueue = nullptr;
}

// 数据写到dst中
void ByteStream::get(NativeByteBuffer *dst) {
    if (dst == nullptr) {
        return;
    }
    for (auto buffer: *bufferQueue) {
        // dst空间不够了
        if (buffer->remaining() > dst->remaining()) {
            dst->writeBytes(buffer, dst->remaining())
        }
    }
}

// 增加一个buffer
void ByteStream::append(NativeByteBuffer *buffer) {
    if (buffer == nullptr) {
        return;
    }
    bufferQueue->push_back(buffer);
}

bool ByteStream::hasData() {
    size_t size = bufferQueue->size();
    for (uint32_t i = 0; i < size; ++i) {
        // todo 注意这里hasRemaining可能存在问题，buffer刚创建的时候hasRemaining是true的
        if (bufferQueue->at(i)->hasRemaining()) {
            return true;
        }
    }
    return false;
}

//
void ByteStream::discard(uint32_t count) {

}

void ByteStream::clear() {
    for (auto buffer: *bufferQueue) {
        // todo
    }
    bufferQueue->clear();
}