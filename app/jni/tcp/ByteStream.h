/** 
 * author: BeggarLan
 * created on: 2023/7/5 16:23 
 * description: 一组buffer
 */

#ifndef BEGGARSOCKETS_BYTESTREAM_H
#define BEGGARSOCKETS_BYTESTREAM_H

#include <stdint.h>
#include <vector>

class NativeByteBuffer;

class ByteStream {
private:
    std::vector<NativeByteBuffer *> *bufferQueue = nullptr;
public:
    ByteStream();
    ~ByteStream();
    ByteStream(const ByteStream &bt);
    // 数据写到dst中
    void get(NativeByteBuffer *dst);
    // 增加一个buffer
    void append(NativeByteBuffer *buffer);
    // buffer中是否有数据
    bool hasData();
    //
    void discard(uint32_t count);
    void clear();
};

#endif //BEGGARSOCKETS_BYTESTREAM_H
