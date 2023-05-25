//
// Created by osia on 05.05.23.
//

#include "DeviceEvent.h"

DeviceEvent::DeviceEvent(std::shared_ptr<Device> device) {
    this->device = device;
}

std::string DeviceEvent::toString() const {
    return "Device " + device->getName() + " event: ";
}
