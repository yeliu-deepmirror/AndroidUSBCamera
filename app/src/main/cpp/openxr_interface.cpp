#include "utils/android_log.h"
#include <jni.h>
#include <cstring>
#include <vector>

JavaVM* vm_ = nullptr;

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  (void)reserved;

  JNIEnv* env;
  if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
    return JNI_ERR;
  }
  vm_ = vm;
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved) {
}

// https://github.com/deepmirrorinc/MobiliService/blob/39b9c7278d81482d4d6013b6ae87a3ace90e4b69/test/jni/car_pose.cc#L173
// Initialize OpenXR
extern "C" JNIEXPORT jlong JNICALL
Java_com_mobili_usbcamera_view_OpenXRInterface_initializeOpenXR(JNIEnv* env, jobject obj, jobject activity, jobject context) {
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
