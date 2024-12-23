// Copyright 2024 DeepMirror Inc. All rights reserved.

#ifndef EXPORT_GRPC_STREAM_API_H_
#define EXPORT_GRPC_STREAM_API_H_

#include <memory>
#include <string>
#include <vector>

namespace dm {

// use grpc to stream images
struct ServiceConfig {
  int port;
  std::string tag = "[MOBILI] ";

  int64_t marker_process_interval = 2e8;
};

void StartService(const ServiceConfig& config);
void StopService();

void PushImage(int64_t timestamp, int width, int height, uint8_t* data, size_t data_length);
void PushImage(int64_t timestamp, int width, int height, uint8_t* data, size_t data_length,
               float fx, float fy, float cx, float cy);

// for marker detection
void SetMarkerWithJPG(float marker_real_width, uint8_t* data, size_t data_length);

}  // namespace dm

#endif  // EXPORT_GRPC_STREAM_API_H_
