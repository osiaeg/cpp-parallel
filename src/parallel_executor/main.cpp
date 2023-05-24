#include <iostream>
#include <memory>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
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
    std::mutex mtx;
    std::condition_variable cv;
};

void EventQueue::push(const std::shared_ptr<const Event> &event) {
//    std::cout << "Add to queue: " << event->toString() << std::endl;
//    mtx.lock();
//    queue.push(event);
//    mtx.unlock();
    std::lock_guard<std::mutex> lock(mtx);
    queue.push(event);
    cv.notify_one();
}

std::shared_ptr<const Event> EventQueue::pop(const std::chrono::seconds &duration) {
//    std::cout << duration.count() << std::endl;
//    std::unique_lock<std::mutex> lock(mtx);
//
//    mtx.lock();
//    std::shared_ptr<const Event> event = queue.front();
//    queue.pop();
//    mtx.unlock();
//    return event;

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] {
        return !queue.empty();
    });
    std::shared_ptr<const Event> res = queue.front();
    queue.pop();
    return res;
}

void readA(std::shared_ptr<DeviceA> device, EventQueue& queue) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
//    std::cout << "readA" << std::endl;
//    std::cout << "Device " << device->getName() << std::endl;
    std::shared_ptr<Event> dataEvent = std::make_shared<DataEvent>(DataEvent(device));
    queue.push(dataEvent);
}

void readB(std::shared_ptr<DeviceB> device, EventQueue& queue) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
//    std::cout << "readB" << std::endl;
//    std::cout << "Device " << device->getName() << std::endl;
    std::shared_ptr<Event> dataEvent = std::make_shared<DataEvent>(DataEvent(device));
    queue.push(dataEvent);
}

//const char NUM_CORE = 4;
int main() {
    EventQueue eventQueue;
    std::shared_ptr<DeviceA> deviceA = std::make_shared<DeviceA>(DeviceA());
    std::shared_ptr<DeviceB> deviceB = std::make_shared<DeviceB>(DeviceB());
    std::thread read_from_DeviceA(readA, deviceA, std::ref(eventQueue));
    std::thread read_from_DeviceB(readB, deviceB, std::ref(eventQueue));
    clock_t start_time = clock();
    while (true) {
        std::cout << std::this_thread::get_id() << std::endl;
        std::shared_ptr<const Event> event = eventQueue.pop(std::chrono::seconds (10));
        if (event) {
            std::cout << event->toString() << std::endl;
            start_time = clock();
        } else {
            continue;
        }
        clock_t spend_time = clock() - start_time;
        std::cout << spend_time << std::endl;
        if (spend_time > 100) {
            read_from_DeviceB.join();
            read_from_DeviceA.join();
            break;
        }
    }
}