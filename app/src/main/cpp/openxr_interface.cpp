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
Java_com_mobili_usbcamera_OpenXRInterface_getDevicePose(JNIEnv* env, jobject obj) {
    // Initialize OpenXR instance
    XrInstance instance;
    XrInstanceCreateInfo instanceCreateInfo = { XR_TYPE_INSTANCE_CREATE_INFO };
    instanceCreateInfo.applicationInfo = { "AppName", 1, "EngineName", 1, XR_CURRENT_API_VERSION };
    xrCreateInstance(&instanceCreateInfo, &instance);

    // Get system
    XrSystemId systemId;
    XrSystemGetInfo systemInfo = { XR_TYPE_SYSTEM_GET_INFO };
    systemInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
    xrGetSystem(instance, &systemInfo, &systemId);

    // Create session
    XrSession session;
    XrSessionCreateInfo sessionCreateInfo = { XR_TYPE_SESSION_CREATE_INFO };
    sessionCreateInfo.systemId = systemId;
    xrCreateSession(instance, &sessionCreateInfo, &session);

    // Create reference space
    XrReferenceSpaceCreateInfo spaceCreateInfo = { XR_TYPE_REFERENCE_SPACE_CREATE_INFO };
    spaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
    spaceCreateInfo.poseInReferenceSpace = { {0, 0, 0, 1}, {0, 0, 0} };
    XrSpace space;
    xrCreateReferenceSpace(session, &spaceCreateInfo, &space);

    // Wait for a frame to get the current time
    XrFrameState frameState = { XR_TYPE_FRAME_STATE };
    XrFrameWaitInfo frameWaitInfo = { XR_TYPE_FRAME_WAIT_INFO };
    xrWaitFrame(session, &frameWaitInfo, &frameState);

    // Use frameState.predictedDisplayTime as the current time
    XrSpaceLocation spaceLocation = { XR_TYPE_SPACE_LOCATION };
    xrLocateSpace(space, space, frameState.predictedDisplayTime, &spaceLocation);

    // Prepare result
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
