/** 
 * author: BeggarLan
 * created on: 2023/7/6 16:25 
 * description: 定时器
 */

#ifndef BEGGARSOCKETS_TIMER_H
#define BEGGARSOCKETS_TIMER_H

#include <stdint.h>
#include <functional>

class EventsDispatcher;

class Timer {
public:
    /**
     * @param callback 超时后触发
     */
    Timer(std::function<void()> callback);
    ~Timer();

    // 连续调用多次无效
    void start();
    // 连续调用多次无效
    void stop();
    // 设置超时时长，如果已经start了那么先移除任务在重新开始即时
    void setTimeoutMs(uint32_t durationMs);

private:
    // 超时后触发
    void onEvent();

    // 超时时长
    uint32_t timeoutDurationMs = 0;
    // 超时后的回调
    std::function<void()> callback;
    // 是否已经开始
    bool isStarted = false;

    // todo
    // 事件分发用
    EventsDispatcher *eventsDispatcher;

    // todo 现在EventsDispatcher的需要关心所有业务，不合理
    friend class EventsDispatcher;
};

#endif //BEGGARSOCKETS_TIMER_H
