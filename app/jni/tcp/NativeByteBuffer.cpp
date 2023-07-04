/** 
 * author: BeggarLan
 * created on: 2023/7/4 16:17 
 * description: ByteBuffer，数据缓冲区实现
 */

#include <stdint.h
#include <string.h>
#include "NativeByteBuffer.h"

NativeByteBuffer::NativeByteBuffer(uint32_t size) {
    buffer = new uint8_t[size];
    bufferOwner = true;
    _limit = _capacity = size;
}

NativeByteBuffer::NativeByteBuffer(bool calculateSizeOnly) {
    this.calculateSizeOnly = calculateSizeOnly
}

NativeByteBuffer::~NativeByteBuffer() {
    if (bufferOwner && buffer != nullptr) {
        delete[] buffer;
        buffer = nullptr;
    }
}

uint32_t NativeByteBuffer::position() {
    return _position
}

void NativeByteBuffer::position(uint32_t position) {
    // 不能超过限制
    if (position > _limit) {
        return;
    }
    _position = position;
}

uint32_t NativeByteBuffer::limit() {
    return _limit;
}

void NativeByteBuffer::limit(uint32_t limit) {
    if (limit > _capacity) {
        return;
    }
    // 注意
    if (_position > limit) {
        _position = limit
    }
    _limit = limit
}

uint32_t NativeByteBuffer::remaining() {
    return _limit - _position
}

bool NativeByteBuffer::hasRemaining() {
    return _position < _limit
}

void NativeByteBuffer::rewind() {
    _position = 0;
}

void NativeByteBuffer::flip() {
    _limit = _position;
    _position = 0;
}

// 转为写入模式
void NativeByteBuffer::compact() {
    memove(buffer, buffer + _position, sizeof(uint8_t) * (_limit - _position))
    _position = -_limit - _position;
    _limit = _capacity;
}

void NativeByteBuffer::skip(uint32_t length) {
    if (!calculateSizeOnly) {
        // 注意跳过的数量超过limit的话 直接return
        if (_position + length > _limit) {
            return;
        }
        _position += length
    } else {
        _capacity += length
    }
}

uint8_t *NativeByteBuffer::bytes() {
    return buffer;
}

uint32_t NativeByteBuffer::capacity() {
    return _capacity;
}

void NativeByteBuffer::clear() {
    _position = 0;
    _limit = _capacity;
}

void NativeByteBuffer::reuse() {

}


// ****************** 写数据 **********************
// ********* 写单个数据
void NativeByteBuffer::writeByte(uint8_t byte) {
    writeByte(byte, nullptr);
}

void NativeByteBuffer::writeInt16(int16_t i) {
    writeInt16(i, nullptr);
}

void NativeByteBuffer::writeInt32(int32_t i) {
    writeInt32(i, nullptr);
}

void NativeByteBuffer::writeInt64(int64_t i) {
    writeInt64(i, nullptr);
}

void NativeByteBuffer::writeDouble(double d) {
    writeDouble(d, nullptr);
}

void NativeByteBuffer::writeBool(bool b) {
    writeBool(b, nullptr);
}

void NativeByteBuffer::writeByte(uint8_t byte, bool *error) {
    if (!calculateSizeOnly) {
        if (_position + 1 > _limit) {
            if (error != nullptr) {
                *error = true;
            }
            return;
        }
        buffer[_position++] = byte;
    } else {
        _capacity += 1;
    }
}

void NativeByteBuffer::writeInt16(int16_t i, bool *error) {
    if (!calculateSizeOnly) {
        if (_position + 2 > _limit) {
            if (error != nullptr) {
                *error = true;
            }
            return;
        }
        buffer[_position++] = (uint8_t)(x >> 8);
        buffer[_position++] = (uint8_t)(x);
    } else {
        _capacity += 2;
    }
}

void NativeByteBuffer::writeInt32(int32_t i, bool *error) {
    if (!calculateSizeOnly) {
        if (_position + 4 > _limit) {
            if (error != nullptr) {
                *error = true;
            }
            return;
        }
        buffer[_position++] = (uint8_t)(x >> 24);
        buffer[_position++] = (uint8_t)(x >> 16);
        buffer[_position++] = (uint8_t)(x >> 8);
        buffer[_position++] = (uint8_t)(x);
    } else {
        _capacity += 4;
    }
}

void NativeByteBuffer::writeInt64(int64_t i, bool *error) {
    if (!calculateSizeOnly) {
        if (_position + 8 > _limit) {
            if (error != nullptr) {
                *error = true;
            }
            return;
        }
        buffer[_position++] = (uint8_t)(x >> 56);
        buffer[_position++] = (uint8_t)(x >> 48);
        buffer[_position++] = (uint8_t)(x >> 40);
        buffer[_position++] = (uint8_t)(x >> 32);
        buffer[_position++] = (uint8_t)(x >> 24);
        buffer[_position++] = (uint8_t)(x >> 16);
        buffer[_position++] = (uint8_t)(x >> 8);
        buffer[_position++] = (uint8_t) x;
    } else {
        _capacity += 8;
    }
}

void NativeByteBuffer::writeDouble(double d, bool *error) {
    // 用64位的int存储
    int64_t v;
    memcpy(&v, &d, sizeof(int64_t));
    writeInt64(v, error);
}

void NativeByteBuffer::writeBool(bool b, bool *error) {
    if (b) {
        writeByte(1, error);
    } else {
        writeByte(0, error);
    }
}

// ******** 写一组数据
void NativeByteBuffer::writeBytes(NativeByteBuffer *buffer) {
    writeBytes(buffer, nullptr);
}

void NativeByteBuffer::writeBytes(uint8_t *buffer, uint32_t length) {
    writeBytes(buffer, 0, length);
}

void NativeByteBuffer::writeBytes(NativeByteBuffer *buffer, bool *error) {
    writeBytes(buffer->bytes(), buffer->position(), buffer->limit())
}

void NativeByteBuffer::writeBytes(uint8_t *buffer, uint32_t length, bool *error) {
    writeBytes(buffer, 0, length, error);
}

// offset: 偏移位置， length：长度
void NativeByteBuffer::writeBytes(uint8_t *bytes, uint32_t offset, uint32_t length) {
    writeBytes(buffer, offset, length, nullptr);
}

void NativeByteBuffer::writeBytes(uint8_t *bytes, uint32_t offset, uint32_t length, bool *error) {
    if (!calculateSizeOnly) {
        if (_position + length > _limit) {
            if (error != nullptr) {
                *error = true;
            }
            return;
        }
        memecpy(buffer + _position, bytes + offset, sizeof(uint8_t) * length);
        _position += length;
    } else {
        _capacity += length
    }
}


// ****************** 读数据 **********************
uint8_t NativeByteBuffer::readByte(bool *error) {
    if (_position + 1 > _limit) {
        if (error != nullptr) {
            *error = true;
        }
        return 0;
    }
    return buffer[_position++];
}

int16_t NativeByteBuffer::readInt16(bool *error) {
    if (_position + 2 > _limit) {
        if (error != nullptr) {
            *error = true;
        }
        return 0;
    }
    uint16_t result = (uint16_t)((buffer[_position] & 0xff) << 8) | (buffer[_position + 1] & 0xff);
    _position += 2;
    return result;
}

int32_t NativeByteBuffer::readInt32(bool *error) {
    if (_position + 4 > _limit) {
        if (error != nullptr) {
            *error = true;
        }
        return 0;
    }
    uint32_t result = (uint32_t)((buffer[_position] & 0xff) << 24) |
                      ((buffer[_position + 1] & 0xff) << 16) |
                      ((buffer[_position + 2] & 0xff) << 8) |
                      ((buffer[_position + 3] & 0xff));
    _position += 4;
    return result;
}

int64_t NativeByteBuffer::readInt64(bool *error) {
    if (_position + 8 > _limit) {
        if (error != nullptr) {
            *error = true;
        }
        return 0;
    }
    int64_t result = ((int64_t)(buffer[_position] & 0xff) << 56) |
                     ((int64_t)(buffer[_position + 1] & 0xff) << 48) |
                     ((int64_t)(buffer[_position + 2] & 0xff) << 40) |
                     ((int64_t)(buffer[_position + 3] & 0xff) << 32) |
                     ((buffer[_position + 4] & 0xff) << 24) |
                     ((buffer[_position + 5] & 0xff) << 16) |
                     ((buffer[_position + 6] & 0xff) << 8) |
                     ((buffer[_position + 7] & 0xff));
    _position += 8;
    return (uint64_t) result;
}

bool NativeByteBuffer::readBool(bool *error) {
    if (_position + 1 > _limit) {
        if (error != nullptr) {
            *error = true;
        }
        return false;
    }
    _position += 1;
    if (buffer[_position] != 0) {
        return true;
    } else {
        return false;
    }
}

double NativeByteBuffer::readDouble(bool *error) {
    double doubleValue;
    int64_t intValue = readInt64(error);
    memcpy(&doubleValue, &intValue, sizeof(double));
    return doubleValue;
}