cmake_minimum_required(VERSION 3.13)
project(untitled1 C)

set(CMAKE_C_STANDARD 11)
find_package(X11 REQUIRED)

message(STATUS "X11_FOUND = ${X11_FOUND}")
message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

include_directories(${X11_INCLUDE_DIR})
link_directories(${X11_LIBRARIES})

file(GLOB ALL_SOURCES ./main.c
        ./utils.c
        ./pulse_circle.c)
file(GLOB ALL_HEADERS ./pulse_circle.h
        ./utils.h)

add_executable(main.out ${ALL_SOURCES} ${ALL_HEADERS})

target_link_libraries(main.out ${X11_LIBRARIES})
