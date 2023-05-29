#include <iostream>
#include <thread>

#include "DeviceA.h"
#include "DeviceB.h"

#include "DataEvent.h"
#include "WorkDoneEvent.h"
#include "StartedEvent.h"

#include "EventQueue.h"


void readA(const std::shared_ptr<DeviceA> &device, EventQueue& queue) {
    for (int i = 0; i < 10; i++) {
        queue.push(std::make_shared<StartedEvent>(StartedEvent(device)));
        queue.push(std::make_shared<DataEvent>(DataEvent(device)));
        std::this_thread::sleep_for(std::chrono::seconds (1));
        queue.push(std::make_shared<WorkDoneEvent>(WorkDoneEvent(device)));
    }
}

void readB(const std::shared_ptr<DeviceB> &device, EventQueue& queue) {
    for (int i = 0; i < 10; i++) {
        queue.push(std::make_shared<StartedEvent>(StartedEvent(device)));
        queue.push(std::make_shared<DataEvent>(DataEvent(device)));
        std::this_thread::sleep_for(std::chrono::seconds (3));
        queue.push(std::make_shared<WorkDoneEvent>(WorkDoneEvent(device)));
    }
}

int main(int argc, char *argv[]) {
    EventQueue eventQueue;


    std::thread read_from_DeviceA(readA,
                                  std::make_shared<DeviceA>(DeviceA()),
                                  std::ref(eventQueue));
    std::thread read_from_DeviceB(readB,
                                  std::make_shared<DeviceB>(DeviceB()),
                                  std::ref(eventQueue));

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