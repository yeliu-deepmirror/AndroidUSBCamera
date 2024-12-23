

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

// public native void passImage(int image_width, int image_height, byte[] byteArray);
extern "C" JNIEXPORT void JNICALL
Java_com_mobili_usbcamera_view_OpenXRInterface_passMarker(JNIEnv* env, jobject obj, jbyteArray byteArray) {
  // void SetMarkerWithJPG(float marker_real_width, uint8_t* data, size_t data_length);
  // Get the length of the byte array
  jsize byteArrayLength = env->GetArrayLength(byteArray);

  // Create a buffer to store the byte array
  char* buffer = new char[byteArrayLength + 1];
  // Copy the byte array into the buffer
  env->GetByteArrayRegion(byteArray, 0, byteArrayLength, reinterpret_cast<jbyte*>(buffer));

  dm::SetMarkerWithJPG(0.27, (uint8_t*)buffer, byteArrayLength);
}

// public native void passImage(int image_width, int image_height, byte[] byteArray);
extern "C" JNIEXPORT void JNICALL
Java_com_mobili_usbcamera_view_OpenXRInterface_passImage(JNIEnv* env, jobject obj, jlong timestamp, jint image_width, jint image_height, jbyteArray byteArray) {

  // Get the length of the byte array
   jsize byteArrayLength = env->GetArrayLength(byteArray);

   // Create a buffer to store the byte array
   char* buffer = new char[byteArrayLength + 1];
   // Copy the byte array into the buffer
   env->GetByteArrayRegion(byteArray, 0, byteArrayLength, reinterpret_cast<jbyte*>(buffer));

   dm::PushImage(timestamp, image_width, image_height, (uint8_t*)buffer, byteArrayLength,
                 600, 600, image_width * 0.5, image_height * 0.5);
}
