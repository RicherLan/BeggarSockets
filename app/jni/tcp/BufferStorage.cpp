/** 
 * author: BeggarLan
 * created on: 2023/7/6 10:29 
 * description: buffer池
 */

#include "BufferStorage.h"
#include "NativeByteBuffer.h"

// buffer池大小
#define FREE_BUFFERS_CAPACITY 10

BufferStorage::BufferStorage(bool threadSafe) {
    isThreadSafe = threadSafe;
    // 初始化5个 8Byte 的buffer
    for (int i = 0; i < 5; ++i) {
        freeBuffers8.push_back(new NativeByteBuffer((uint32_t) 8))
    }
    // 初始化5个 128Byte 的buffer
    for (int i = 0; i < 5; ++i) {
        freeBuffers128.push_back(new NativeByteBuffer((uint32_t) 128))
    }
}

BufferStorage &BufferStorage::getInstance() {
    // 局部静态变量，只初始化一次，且c++11是线程安全的(第一次初始化的时候，其他线程会等待)，注意c++11以下线程不安全
    static BufferStorage storage(true);
    return storage;
}

NativeByteBuffer *BufferStorage::getBuffer(uint32_t size) {
    // 从哪个池子中获取
    std::vector < NativeByteBuffer * > bufferGetFrom = nullptr;
    // 池子中拿不到的话，就手动创建
    NativeByteBuffer *buffer = nullptr;
    // buffer的大小
    uint32_t byteCount = 0;

    if (size <= 8) {
        bufferGetFrom = &freeBuffers8;
        byteCount = 8;
    } else if (size <= 128) {
        bufferGetFrom = &freeBuffers128;
        byteCount = 128;
    } else if (size <= 1024) {
        bufferGetFrom = &freeBuffers1024;
        byteCount = 1024;
    } else if (size <= 4 * 1024) {
        bufferGetFrom = &freeBuffers4k;
        byteCount = 4 * 1024;
    } else if (size <= 16 * 1024) {
        bufferGetFrom = &freeBuffers16k;
        byteCount = 16 * 1024;
    } else if (size <= 32 * 1024) {
        bufferGetFrom = &freeBuffers32k;
        byteCount = 32 * 1024;
    } else if (size <= 128 * 1024) {
        bufferGetFrom = &freeBuffers128k;
        byteCount = 128 * 1024;
    } else {
        buffer = new NativeByteBuffer(size);
    }

    // 从池之中取
    if (bufferGetFrom != nullptr) {
        std::lock_guard <std::mutex> lock(storageMutex);

        if (bufferGetFrom->size() > 0) {
            buffer = (*bufferGetFrom)[0];
            bufferGetFrom->erase(bufferGetFrom->begin());
        }

        // 池子中没有了
        if (buffer == nullptr) {
            // 创建符合对应池大小的buffer，因为要循环利用(释放的时候扔到池子)
            buffer = new NativeByteBuffer(byteCount);
        }
    }

    buffer->clear();
    return buffer;
}

// todo 这里有一个问题，比如buffer的大小小于8，那么会扔到freeBuffers8中，再次取的时候(想要8)拿到的buffer是小于8的不够用
// todo 先不优化：因为只要获取buffer都经过storage就不会有问题
void BufferStorage::reuseFreeBuffer(NativeByteBuffer *buffer) {
    if (buffer == nullptr) {
        return;
    }
    uint32_t capacity = buffer->capacity();
    // buffer池容量
    uint32_t maxSize = FREE_BUFFERS_CAPACITY;
    std::vector < NativeByteBuffer * > *bufferPool = nullptr;
    if (capacity <= 8) {
        bufferPool = &freeBuffers8;
        maxSize = 80;
    } else if (capacity <= 128) {
        bufferPool = &freeBuffers128;
        maxSize = 80;
    } else if (capacity <= 1024) {
        bufferPool = &freeBuffers1024;
    } else if (capacity <= 4 * 1024) {
        bufferPool = &freeBuffers4k;
    } else if (capacity <= 16 * 1024) {
        bufferPool = &freeBuffers16k;
    } else if (capacity <= 32 * 1024) {
        bufferPool = &freeBuffers32k;
    } else if (capacity <= 128 * 1024) {
        bufferPool = &freeBuffers128k;
    }

    // 往池子里面放，放不下(到达容量)就释放buffer
    if (bufferPool != nullptr) {
        std::lock_guard <std::mutex> lock(storageMutex);
        if (bufferPool->size() < maxSize) {
            bufferPool->push_back(buffer);
        } else {
            // 放不下(到达容量)就释放buffer
            delete buffer;
        }
    } else {
        // 没有对应的池子
        delete buffer;
    }
}