/** 
 * author: BeggarLan
 * created on: 2023/7/6 18:02 
 * description: time工具类
 */

#include "stdint.h"
#include <time.h>

// 真实时间
int64_t getCurrentTimeMillis() {
    timespec timeSpec;
    clock_gettime(CLOCK_REALTIME, &timeSpec);
    return (int64_t) timeSpec.tv_sec * 1000 + (int64_t) timeSpec.tv_nsec / 1000000;
}

// 系统启动以后经过的时间，由变量jiffies来记录的
int64_t getCurrentTimeMonotonicMillis() {
    timespec timeSpec;
    clock_gettime(CLOCK_MONOTONIC, &timeSpec);
    return (int64_t) timeSpec.tv_sec * 1000 + (int64_t) timeSpec.tv_nsec / 1000000;
}