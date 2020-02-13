cmake_minimum_required(VERSION 3.12)
project(Codes)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Codes
    Calibration.cpp
    Calibration.hpp
    main.cpp)
find_package(OpenCV REQUIRED)
target_link_libraries(Codes ${OpenCV_LIBS})