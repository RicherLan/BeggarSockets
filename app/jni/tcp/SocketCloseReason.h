/** 
 * author: BeggarLan
 * created on: 2023/7/7 12:53 
 * description: socket关闭的原因
 */

#ifndef BEGGARSOCKETS_SOCKETCLOSEREASON_H
#define BEGGARSOCKETS_SOCKETCLOSEREASON_H

enum SocketCloseReason {
    COMMON = 1,   // 暂时不分太细
    TIMEOUT = 2,  // 超时
    MANUAL = 3,   // 主动关闭的
};

#endif //BEGGARSOCKETS_SOCKETCLOSEREASON_H
