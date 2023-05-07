cmake_minimum_required(VERSION 3.15.0)
cmake_policy(SET CMP0016 NEW)

project(parallel_executor VERSION 1.0.0 DESCRIPTION "Синхронизация нескольких потоков исполнения через очередь сообщений на C++")

include_directories(${CMAKE_SOURCE_DIR}/src)
file(GLOB Device_Event_SRC
        "src/parallel_executor/Device_Event/*.h"
        "src/parallel_executor/Device_Event/*.cpp"
        )
add_library(device ${Device_Event_SRC})

add_executable(${PROJECT_NAME} src/parallel_executor/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC src/parallel_executor/Device_Event)
target_link_libraries(${PROJECT_NAME} device)

enable_testing()
if (BUILD_TESTING)
    add_subdirectory(test)
endif ()
