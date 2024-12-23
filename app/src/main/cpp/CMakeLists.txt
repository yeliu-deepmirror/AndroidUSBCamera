cmake_minimum_required(VERSION 3.10.2)


find_library(log-lib log)

include_directories(${CMAKE_SOURCE_DIR}/grpc_stream_api/include)
add_library(grpc_stream_api SHARED IMPORTED)
set_target_properties(grpc_stream_api PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/grpc_stream_api/${ANDROID_ABI}/libgrpc_stream_api.so)

add_library(opencv SHARED IMPORTED)
set_target_properties(opencv PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/grpc_stream_api/${ANDROID_ABI}/libopencv_java4.so)


include_directories(${CMAKE_SOURCE_DIR})


add_library(openxr_interface SHARED openxr_interface.cpp)

target_link_libraries(openxr_interface
  ${log-lib}
  grpc_stream_api
  opencv
)
