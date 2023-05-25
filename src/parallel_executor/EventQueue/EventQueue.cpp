//
// Created by osia on 25.05.23.
//

#include "EventQueue.h"

void EventQueue::push(const std::shared_ptr<const Event> &event) {
    std::lock_guard<std::mutex> lock(mtx);
    queue.push(event);
    cv.notify_one();
}

std::shared_ptr<const Event> EventQueue::pop(const std::chrono::seconds &duration) {
    std::unique_lock<std::mutex> lock(mtx);
    auto endTime = std::chrono::steady_clock::now() + duration;

    while (queue.empty()) {
        auto res = cv.wait_until(lock, endTime);
        if (res == std::cv_status::timeout) {
            return nullptr;
        }
    }

    std::shared_ptr<const Event> res = queue.front();
    queue.pop();
    return res;
}
