cmake_minimum_required(VERSION 4.0.0)

project(Log_html)

include_directories(include/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(program src/main.cpp src/parselib.cpp include/parselib.h)

