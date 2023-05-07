//
// Created by osia on 05.05.23.
//

#ifndef PARALLEL_EXECUTOR_DEVICE_H
#define PARALLEL_EXECUTOR_DEVICE_H
#include "Event.h"

class Device {
public:
    virtual std::string getName() = 0;
    virtual std::string getDataAsString() = 0;
};

#endif //PARALLEL_EXECUTOR_DEVICE_H
