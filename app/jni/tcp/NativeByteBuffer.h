/** 
 * author: BeggarLan
 * created on: 2023/7/4 13:42 
 * description: ByteBuffer，数据缓冲区
 */

#ifndef BEGGARSOCKETS_NATIVEBYTEBUFFER_H
#define BEGGARSOCKETS_NATIVEBYTEBUFFER_H

#include <stdint.h>
#include <string>

// android平台引入jni
#indef ANDROID
#include <jni.h>
#endif

/**
 * 使用流程：
 *    初始化
 * –> 写入数据（write）
 * –> 转换为读取模式（flip）
 * –> 读取数据（read）
 * –> 转换为写入模式（compact）
 * –> 写入数据（write）
 * -> ......
 */
class NativeByteBuffer {
private:
    // 缓冲区
    uint8_t *buffer;

    // 0 <= position <= limit <= capacity
    // 当前的下标位置，表示进行下一个读写操作时的起始位置
    uint32_t _position = 0;
    // 结束位置,表示进行下一个读写操作时的（最大）结束位置
    uint32_t _limit = 0;
    // 该buffer容量
    uint32_t _capacity = 0;

    // 不存储数据，仅用来计算大小：1字节数据(8位)对应1长度
    bool calculateSizeOnly = false;
    bool bufferOwner = true;

#ifdef ANDROID
    // java层的ByteBuffer
    // todo
    jobject javaByteBuffer = nullptr;
#endif

public:
    // buffer大小
    NativeByteBuffer(uint32_t size);
    // 不存储数据，仅用来计算大小：1字节数据(8位)对应1长度
    NativeByteBuffer(bool calculateSizeOnly);
    ~NativeByteBuffer();

    // 当前指针位置
    uint32_t position();
    // 移动指针位置
    void position(uint32_t position);

    uint32_t limit();
    void limit(uint32_t limit);

    // 当前剩余的可用长度： limit - position
    uint32_t remaining();
    bool hasRemaining();

    // 将position复原到0
    void rewind();
    /**
     * 转换为读取模式
     * 1. 把limit的值设置为当前的position的值
     * 2. position的值设置为0
     */
    void flip();
    /**
     * 转换为写入模式
     * 1. 将 position 到 limit 之间还未读取的数据拷贝到 ByteBuffer 中数组的最前面，
     * 2. 然后再将 position 移动至这些数据之后的一位，将 limit 移动至 capacity
     */
    void compact();
    // 跳过xx字节
    void skip(uint32_t length);

    uint8_t *bytes();

    uint32_t capacity();
    void clearCapacity();
    void clear();

    void reuse();

    // ****************** 写数据 **********************
    // ********* 写单个数据
    void writeByte(uint8_t byte);
    void writeInt16(int16_t i);
    void writeInt32(int32_t i);
    void writeInt64(int64_t i);
    void writeDouble(double d);
    void writeBool(bool b);

    void writeByte(uint8_t byte, bool *error);
    void writeInt16(int16_t i, bool *error);
    void writeInt32(int32_t i, bool *error);
    void writeInt64(int64_t i, bool *error);
    void writeDouble(double d, bool *error);
    void writeBool(bool b, bool *error);

    // ******** 写一组数据
    void writeBytes(NativeByteBuffer *buffer);
    void writeBytes(uint8_t *buffer, uint32_t length);
    // offset: 偏移位置， length：长度
    void writeBytes(uint8_t *buffer, uint32_t offset, uint32_t length);

    void writeBytes(NativeByteBuffer *buffer, bool *error);
    void writeBytes(uint8_t *buffer, uint32_t length, bool *error);
    void writeBytes(uint8_t *buffer, uint32_t offset, uint32_t length, bool *error);


    // ****************** 读数据 **********************
    uint8_t  readByte(bool *error);
    int16_t  readInt16(bool *error);
    int32_t  readInt32(bool *error);
    int64_t  readInt64(bool *error);
    uint16_t readUint16(bool *error);
    uint32_t readUint32(bool *error);
    uint64_t readUint64(bool *error);
    bool readBool(bool *error);
    double readDouble(bool *error);


#ifdef ANDROID
    // 获得java层的ByteBuffer
    // todo
    jobject getJavaByteBuffer();
#endif
};

#endif //BEGGARSOCKETS_NATIVEBYTEBUFFER_H
