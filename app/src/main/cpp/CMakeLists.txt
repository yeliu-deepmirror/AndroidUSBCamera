cmake_minimum_required(VERSION 3.10.2)

add_library(openxr_interface SHARED openxr_interface.cpp)

find_library(log-lib log)

target_link_libraries(openxr_interface ${log-lib})
