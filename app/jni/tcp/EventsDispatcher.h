/** 
 * author: BeggarLan
 * created on: 2023/7/5 10:07 
 * description: 事件分发
 * todo 应该采用注册回调的方式
 */

#ifndef BEGGARSOCKETS_EVENTSDISPATCHER_H
#define BEGGARSOCKETS_EVENTSDISPATCHER_H

#include "stdint.h"
#include "TcpConstant.h"

class EventsDispatcher {
public:
    // 事件类型
    EventType eventType;
    // 将事件分发给这个object
    void *eventObject;
    // 用来记录时间用
    int64_t time;

    // 关联【接收事件的对象】 和 【该对象关心的事件类型】
    EventsDispatcher(void *object, EventType type);

    /**
     * 收到事件了
     * @param events 所有的事件
     */
    void onEvent(uint32_t events);
};

#endif //BEGGARSOCKETS_EVENTSDISPATCHER_H
