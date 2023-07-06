/** 
 * author: BeggarLan
 * created on: 2023/7/6 16:36 
 * description: 计时器
 */

#include "Timer.h"
#include "EventsDispatcher.h"

Timer::Timer(std::function<void()> callback) {
    this->callback = callback;
    eventsDispatcher = new EventsDispatcher(this, EventType::EventTimer);
}

Timer::~Timer() {
    stop();
    if (eventsDispatcher != nullptr) {
        delete eventsDispatcher;
        eventsDispatcher = nullptr;
    }
}

void Timer::setTimeoutMs(uint32_t durationMs) {
    if (durationMs <= 0) {
        return;
    }
    timeoutDurationMs = durationMs;
    // 如果已经开始了那么先移除任务在重新开始即时
    if (isStarted) {
        // remove;
        // schedule;
    }
}

void Timer::start() {
    if (isStarted || timeoutDurationMs < 0) {
        return;
    }
    isStarted = true;
    // schedule;
}

void Timer::stop() {
    if (!isStarted) {
        return;
    }
    isStarted = false;
    // remove;
}

void Timer::onEvent() {
    if (callback != nullptr) {
        callback();
    }
}