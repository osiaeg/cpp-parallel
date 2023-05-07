#include <iostream>
#include <memory>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "DataEvent.h"
#include "DeviceA.h"
#include "DeviceB.h"
#include "WorkDoneEvent.h"
#include "StartedEvent.h"

class EventQueue {
public:
    void push(const std::shared_ptr<const Event>& event);

    std::shared_ptr<const Event> pop(const std::chrono::seconds& duration);
private:
    std::queue<std::shared_ptr<const Event>> queue;
//    std::mutex mtx;
    std::condition_variable cv;
};

void EventQueue::push(const std::shared_ptr<const Event> &event) {
    std::cout << "Add to queue: " << event->toString() << std::endl;
    queue.push(event);
}

std::shared_ptr<const Event> EventQueue::pop(const std::chrono::seconds &duration) {
    std::cout << duration.count() << std::endl;
    std::shared_ptr<const Event> event = queue.front();
    queue.pop();
    return event;
}

void readA() {}

//const char NUM_CORE = 4;
int main() {
//    EventQueue queue;
//    std::chrono::duration<int> one_second(1);
//    std::chrono::duration<int> five_second(5);
//
//    std::shared_ptr<DeviceA> deviceA = std::make_shared<DeviceA>();
//    std::shared_ptr<DeviceB> deviceB = std::make_shared<DeviceB>();
//
//    std::shared_ptr<Event> dataEvent = std::make_shared<DataEvent>(DataEvent(deviceB));
//    std::shared_ptr<Event> workDoneEvent = std::make_shared<WorkDoneEvent>(WorkDoneEvent(deviceA));
//    std::shared_ptr<Event> startedEvent = std::make_shared<StartedEvent>(StartedEvent(deviceB));
//
//    queue.push(startedEvent);
//    queue.push(dataEvent);
//    queue.push(workDoneEvent);
//
//    std::cout << queue.pop(one_second)->toString() << std::endl;
//    std::cout << queue.pop(five_second)->toString() << std::endl;
//    std::cout << queue.pop(one_second)->toString() << std::endl;

    return 0;
}