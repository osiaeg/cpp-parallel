//
// Created by osia on 05.05.23.
//

#include "DataEvent.h"

std::string DataEvent::toString() const {
    return DeviceEvent::toString() + "Data: " + device->getDataAsString();
}

