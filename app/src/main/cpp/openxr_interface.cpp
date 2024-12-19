

#include "common/redirect_android_log.h"
#include <jni.h>
#include <cstring>
#include <vector>
#include "grpc_stream_api.h"

JavaVM* vm_ = nullptr;

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  (void)reserved;

  JNIEnv* env;
  if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
    return JNI_ERR;
  }
  vm_ = vm;

  dm::xr::RunLoggingThread();
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved) {
  dm::StopService();
}

// Initialize
extern "C" JNIEXPORT jlong JNICALL
Java_com_mobili_usbcamera_view_OpenXRInterface_initialize(JNIEnv* env, jobject obj, jobject activity, jobject context) {

  LOGI("OpenXRInterface_initialize");

  dm::ServiceConfig config;
  config.port = 15212;
  dm::StartService(config);

  return 0;
}

// Retrieve pose data
extern "C" JNIEXPORT jfloatArray JNICALL
Java_com_mobili_usbcamera_view_OpenXRInterface_getDevicePose(JNIEnv* env, jobject obj) {
  // Prepare result
  jfloatArray resultArray = env->NewFloatArray(7);
  jfloat poseData[7] = {0, 0, 0, 0, 0, 0, 0};
  env->SetFloatArrayRegion(resultArray, 0, 7, poseData);
  return resultArray;
}
