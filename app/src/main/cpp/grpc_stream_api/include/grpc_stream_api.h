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
};

void StartService(const ServiceConfig& config);
void StopService();


void PushImage(int64_t timestamp, int width, int height, uint8_t* data, size_t data_length);

}  // namespace dm

#endif  // EXPORT_GRPC_STREAM_API_H_
