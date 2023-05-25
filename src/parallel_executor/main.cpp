#include <iostream>
#include <thread>
#include "DataEvent.h"
#include "DeviceA.h"
#include "DeviceB.h"
#include "WorkDoneEvent.h"
#include "StartedEvent.h"
#include "EventQueue.h"


void readA(std::shared_ptr<DeviceA> device, EventQueue& queue) {
    std::shared_ptr<Event> startedEvent = std::make_shared<StartedEvent>(StartedEvent(device));
    queue.push(startedEvent);
    std::this_thread::sleep_for(std::chrono::seconds (1));
    std::shared_ptr<Event> dataEvent = std::make_shared<DataEvent>(DataEvent(device));
    queue.push(dataEvent);
    std::this_thread::sleep_for(std::chrono::seconds (1));
    std::shared_ptr<Event> workDoneEvent = std::make_shared<WorkDoneEvent>(WorkDoneEvent(device));
    queue.push(workDoneEvent);
    std::this_thread::sleep_for(std::chrono::seconds (1));
}

void readB(std::shared_ptr<DeviceB> device, EventQueue& queue) {
    std::shared_ptr<Event> startedEvent = std::make_shared<StartedEvent>(StartedEvent(device));
    queue.push(startedEvent);
    std::this_thread::sleep_for(std::chrono::seconds (3));
    std::shared_ptr<Event> dataEvent = std::make_shared<DataEvent>(DataEvent(device));
    queue.push(dataEvent);
    std::this_thread::sleep_for(std::chrono::seconds (3));
    std::shared_ptr<Event> workDoneEvent = std::make_shared<WorkDoneEvent>(WorkDoneEvent(device));
    queue.push(workDoneEvent);
    std::this_thread::sleep_for(std::chrono::seconds (3));
}

int main() {
    EventQueue eventQueue;
    std::shared_ptr<DeviceA> deviceA = std::make_shared<DeviceA>(DeviceA());
    std::shared_ptr<DeviceB> deviceB = std::make_shared<DeviceB>(DeviceB());
    std::thread read_from_DeviceA(readA, deviceA, std::ref(eventQueue));
    std::thread read_from_DeviceB(readB, deviceB, std::ref(eventQueue));
    while (true) {
        std::shared_ptr<const Event> event = eventQueue.pop(std::chrono::seconds (5));
        if (event) {
            std::cout << event->toString() << std::endl;
        } else {
            read_from_DeviceA.join();
            read_from_DeviceB.join();
            break;
        }
    }
}