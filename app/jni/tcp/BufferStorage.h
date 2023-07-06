/** 
 * author: BeggarLan
 * created on: 2023/7/6 10:17 
 * description: buffer池:
 * 提供：获取buffer + 释放buffer
 */

#ifndef BEGGARSOCKETS_BUFFERSTORAGE_H
#define BEGGARSOCKETS_BUFFERSTORAGE_H

#include "stdint.h"
#include "vector"
#include "mutex"

class NativeByteBuffer;

class BufferStorage {
public:
    static BufferStorage &getInstance();

    /**
     * 获取buffer
     * @param size 要获取的buffer的大小
     */
    NativeByteBuffer *getBuffer(uint32_t size);

    /**
     * 用完了释放buffer
     */
    void reuseFreeBuffer(NativeByteBuffer *buffer);

private:
    /*
     * @param threadSafe 是否线程安全
     */
    BufferStorage(bool threadSafe);

    // 不同大小的buffer的池子
    std::vector<NativeByteBuffer *> freeBuffers8;
    std::vector<NativeByteBuffer *> freeBuffers128;
    std::vector<NativeByteBuffer *> freeBuffers1024;
    std::vector<NativeByteBuffer *> freeBuffers4k;
    std::vector<NativeByteBuffer *> freeBuffers16k;
    std::vector<NativeByteBuffer *> freeBuffers32k;
    std::vector<NativeByteBuffer *> freeBuffers128k;

    // 是否线程安全
    bool isThreadSafe;
    // 锁
    std::mutex storageMutex
};

#endif //BEGGARSOCKETS_BUFFERSTORAGE_H
