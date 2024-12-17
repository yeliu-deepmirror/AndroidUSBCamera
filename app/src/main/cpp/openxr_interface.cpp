#include <jni.h>
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>


extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  // LOGI("JNI_OnLoad");
  // dm::xr::RunLoggingThread();
  // g_camera = new MyNdkCamera;
  return JNI_VERSION_1_4;
}

extern "C" JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved) {
  // LOGI("JNI_OnUnload");
  // mobili::api::hud::Shutdown();
  //
  // delete g_camera;
  // g_camera = 0;
}


extern "C" JNIEXPORT jfloatArray JNICALL
Java_com_mobili_usbcamera_view_USBCameraActivity_getDevicePose(JNIEnv* env, jobject obj) {
    // Initialize OpenXR instance
    XrInstance instance;
    XrInstanceCreateInfo instanceCreateInfo = { XR_TYPE_INSTANCE_CREATE_INFO };
    xrCreateInstance(&instanceCreateInfo, &instance);

    // Get system ID
    XrSystemId systemId;
    XrSystemGetInfo systemInfo = { XR_TYPE_SYSTEM_GET_INFO };
    systemInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
    xrGetSystem(instance, &systemInfo, &systemId);

    // Retrieve the current pose
    XrSpace space; // A space to track device position/orientation
    XrSpaceCreateInfo spaceCreateInfo = { XR_TYPE_SPACE_CREATE_INFO };
    xrCreateSpace(instance, &spaceCreateInfo, &space);

    XrPosef pose;
    XrTime currentTime;
    XrSpaceLocation spaceLocation = { XR_TYPE_SPACE_LOCATION };
    xrLocateSpace(space, space, currentTime, &spaceLocation);

    // Return pose to Java
    jfloatArray result = env->NewFloatArray(7);
    jfloat poseData[7] = {
        spaceLocation.pose.orientation.x,
        spaceLocation.pose.orientation.y,
        spaceLocation.pose.orientation.z,
        spaceLocation.pose.orientation.w,
        spaceLocation.pose.position.x,
        spaceLocation.pose.position.y,
        spaceLocation.pose.position.z
    };
    env->SetFloatArrayRegion(result, 0, 7, poseData);
    return result;
}
