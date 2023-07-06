/** 
 * author: BeggarLan
 * created on: 2023/7/5 10:16 
 * description: 事件分发
 */

#include "stdint.h"
#include <sys/eventfd.h>
#include "EventsDispatcher.h"
#include "Timer.h"

EventsDispatcher::EventsDispatcher(void *object, EventType type) {
    eventObject = object;
    eventType = type;
}

// todo 耦合了使用方，一个回调就能解决的问题
void EventsDispatcher::onEvent(uint32_t events) {
    switch (eventType) {
        case EventType::EventConnection: {
            // 连接

            break;
        }
        case EventType::EventPipe: {
            // 管道

            break;
        }
        case EventType::EventFd: {
            // EventFd

            break;
        }
        case EventType::EventTimer: {
            // 定时器
            Timer *timer = (Timer *) eventObject;
            timer->onEvent();
            break;
        }
        default: {

        }
    }
}