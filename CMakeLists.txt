cmake_minimum_required(VERSION 3.13)
project(lab_3 C)
find_package(X11 REQUIRED)

message(STATUS "X11_FOUND = ${X11_FOUND}")
message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")

set(CMAKE_C_STANDARD 11)

include_directories(${X11_INCLUDE_DIR})
link_directories(${X11_LIBRARIES})

file(GLOB SOURCES ./lastmain.c
        ./lastf.c)

file(GLOB HEADERS ./lasth.h)

add_executable(main.out ${SOURCES} ${HEADERS})

target_link_libraries(main.out ${X11_LIBRARIES})