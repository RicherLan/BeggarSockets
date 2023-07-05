/** 
 * author: BeggarLan
 * created on: 2023/7/5 10:01 
 * description: 放一些常量、枚举等，大杂烩，非常不合理
 */

#ifndef BEGGARSOCKETS_TCPCONSTANT_H
#define BEGGARSOCKETS_TCPCONSTANT_H

#include <stdint.h>

/**
 * 所有的event
 */
enum EventType {
    Connection, // 链接
    Pip,        // pipe
    Fd,         // eventFd
    Timer,      // 计时器
};

#endif //BEGGARSOCKETS_TCPCONSTANT_H
