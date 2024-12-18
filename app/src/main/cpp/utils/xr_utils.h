// Copyright 2022 DeepMirror Inc. All rights reserved.

#ifndef COMMON_XR_UTILS_H_
#define COMMON_XR_UTILS_H_

#include <vector>
#include "android_log.h"
#include <android/native_activity.h>
#define XR_USE_PLATFORM_ANDROID

#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>
#include <openxr/openxr_reflection.h>



bool XrCheck(XrInstance instance, XrResult result, const char* format, ...) {
  if (XR_SUCCEEDED(result)) return true;

  char resultString[XR_MAX_RESULT_STRING_SIZE];
  xrResultToString(instance, result, resultString);

  LOGE("%s [%s] (%d)", format, resultString, result);
  return false;
}


bool PrintAPILayer() {
  uint32_t count = 0;
  XrResult result = xrEnumerateApiLayerProperties(0, &count, NULL);
  if (!XrCheck(NULL, result, "Failed to enumerate api layer count")) {
    return false;
  }

  if (count == 0) {
    return true;
  }

  std::vector<XrApiLayerProperties> props(count);
  for (uint32_t i = 0; i < count; i++) {
    props[i].type = XR_TYPE_API_LAYER_PROPERTIES;
    props[i].next = NULL;
  }

  result = xrEnumerateApiLayerProperties(count, &count, props.data());
  if (!XrCheck(NULL, result, "Failed to enumerate api layers")) {
    return false;
  }

  LOGI("API layers:");
  for (uint32_t i = 0; i < count; i++) {
    LOGI("%s %d %s", props[i].layerName, props[i].layerVersion, props[i].description);
  }
  return true;
}

bool PrintExt() {
  uint32_t ext_count = 0;
  XrResult result = xrEnumerateInstanceExtensionProperties(NULL, 0, &ext_count, NULL);
  if (!XrCheck(NULL, result, "Failed to enumerate number of extension properties")) {
    return false;
  }

  std::vector<XrExtensionProperties> ext_props(ext_count);
  for (uint16_t i = 0; i < ext_count; i++) {
    ext_props[i].type = XR_TYPE_EXTENSION_PROPERTIES;
    ext_props[i].next = NULL;
  }

  result = xrEnumerateInstanceExtensionProperties(NULL, ext_count, &ext_count, ext_props.data());
  if (!XrCheck(NULL, result, "Failed to enumerate extension properties")) {
    return false;
  }

  LOGI("Runtime supports extensions count: %d", ext_count);
  // LOG(INFO) << "Runtime supports extensions count: " << ext_count;
  for (uint32_t i = 0; i < ext_count; i++) {
    LOGI(" %s, %d", ext_props[i].extensionName, ext_props[i].extensionVersion);
  }
  return true;
}

#endif  // COMMON_XR_UTILS_H_
