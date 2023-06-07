#include <iostream>
#include <thread>

#include "DeviceA.h"
#include "DeviceB.h"

#include "DataEvent.h"
#include "WorkDoneEvent.h"
#include "StartedEvent.h"
#include "EventQueue.h"

void read(const std::shared_ptr<Device> &device, EventQueue &queue, std::chrono::seconds time, int cycles) {
    queue.push(std::make_shared<StartedEvent>(StartedEvent(device)));
    for (int i = 0; i < cycles; i++) {
        queue.push(std::make_shared<DataEvent>(DataEvent(device)));
        std::this_thread::sleep_for(time);
    }
    queue.push(std::make_shared<WorkDoneEvent>(WorkDoneEvent(device)));
}

int main(int argc, char *argv[]) {
    EventQueue eventQueue;
    if (argc != 3) {
        std::cerr << "Usage: paraller_executor <cycles_of_DeviceA> <cycles_of_DeviceB>\n";
        return 1;
    }

    int cyclesA = std::stoi(argv[1]);
    int cyclesB = std::stoi(argv[2]);


    std::thread read_from_DeviceA(read,
                                  std::make_shared<DeviceA>(DeviceA()),
                                  std::ref(eventQueue),
                                  std::chrono::seconds(1),
                                  cyclesA);
    std::thread read_from_DeviceB(read,
                                  std::make_shared<DeviceB>(DeviceB()),
                                  std::ref(eventQueue),
                                  std::chrono::seconds(5),
                                  cyclesB);

    while (true) {
        std::shared_ptr<const Event> event = eventQueue.pop(std::chrono::seconds (6));
        if (event) {
            std::cout << event -> toString() << std::endl;
        } else {
            read_from_DeviceA.join();
            read_from_DeviceB.join();
            break;
        }
    }
}