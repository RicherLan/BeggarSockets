/** 
 * author: BeggarLan
 * created on: 2023/7/6 16:12 
 * description: 包头定义
 */

#ifndef BEGGARSOCKETS_PACKETHEADER_H
#define BEGGARSOCKETS_PACKETHEADER_H

#include "stdint.h"

// 1字节对齐(编译器默认8字节对齐)
#pragma pack(push, 1)
typedef struct PacketHeader {
    uint16_t pkgLen;  // 包大小
    uint16_t headLen; // 头大小
    uint8_t version;  // 协议版本
    int32_t cmd;      // 命令
    int64_t uin;      // 唯一账号标识
    int32_t  seq;     // 包的seq
} PacketHeader;
#pragma pack(pop)

#endif //BEGGARSOCKETS_PACKETHEADER_H
