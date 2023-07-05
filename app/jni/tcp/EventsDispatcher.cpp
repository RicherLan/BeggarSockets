/** 
 * author: BeggarLan
 * created on: 2023/7/5 10:16 
 * description: 事件分发
 */

#include "stdint.h"
#include <sys/eventfd.h>
#include "EventsDispatcher.h"

EventsDispatcher::EventsDispatcher(void *object, EventType type) {
    eventObject = object;
    eventType = type;
}

// todo 耦合了使用方，一个回调就能解决的问题
void EventsDispatcher::onEvent(uint32_t events) {
    switch (eventType) {
        // 连接
        case EventType::Connection: {

            break;
        }
        // 管道
        case EventType::Pipe: {

            break;
        }
        // EventFd
        case EventType::Fd: {

            break;
        }
        // 定时器
        case EventType::Timer: {

            break;
        }
        default: {

        }
    }
}