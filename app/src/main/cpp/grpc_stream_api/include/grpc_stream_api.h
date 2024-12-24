// Copyright 2024 DeepMirror Inc. All rights reserved.

#ifndef EXPORT_GRPC_STREAM_API_H_
#define EXPORT_GRPC_STREAM_API_H_

#include <memory>
#include <string>
#include <vector>

namespace dm {

// use grpc to stream images
struct ServiceConfig {
  int grpc_port;
  int websocket_port;
  std::string tag = "[MOBILI] ";

  int64_t marker_process_interval = 2e8;
};

void StartService(const ServiceConfig& config);
// for marker detection
void SetMarkerWithJPG(float marker_real_width, uint8_t* data, size_t data_length);

void StopService();

void PushImage(int64_t timestamp, int width, int height, uint8_t* data, size_t data_length);
void PushImage(int64_t timestamp, int width, int height, uint8_t* data, size_t data_length,
               float fx, float fy, float cx, float cy);

// -2 : not initialized
// -1 : image not received yet
// 0 : marker not localized yet
// 1 : estimator got nothing
// 2 : XR pose received but not localized yet
// 3 : XR pose received and localize success
int GetCurrentStatus();

// get the latest marker location, used for debug render
bool GetLatestLocalization(int64_t* timestamp, std::vector<std::pair<float, float>>* points);

}  // namespace dm

#endif  // EXPORT_GRPC_STREAM_API_H_
