cmake_minimum_required(VERSION 3.10.2)


add_library(openxr_loader SHARED IMPORTED)
set_target_properties(openxr_loader PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/openxr_loader/openxr_loader/libs/android.${ANDROID_ABI}/libopenxr_loader.so)

include_directories(${CMAKE_SOURCE_DIR}/openxr_loader/headers/include)
add_library(openxr_interface SHARED openxr_interface.cpp)

find_library(log-lib log)

target_link_libraries(openxr_interface ${log-lib} openxr_loader)
