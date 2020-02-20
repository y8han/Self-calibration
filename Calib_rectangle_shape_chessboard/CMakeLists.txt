cmake_minimum_required(VERSION 3.12)
project(calib)

set(CMAKE_CXX_STANDARD 14)
add_executable(calib
        main.cpp)
find_package(OpenCV REQUIRED)
target_link_libraries(calib ${OpenCV_LIBS})