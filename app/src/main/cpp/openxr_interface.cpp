#include "utils/android_log.h"
#include "utils/xr_utils.h"
#include <jni.h>
#include <cstring>
#include <vector>

JavaVM* vm_ = nullptr;
static PFN_xrInitializeLoaderKHR xrInitializeLoaderKHR = NULL;

XrInstance m_instance_ = XR_NULL_HANDLE;
XrSession m_session_ = XR_NULL_HANDLE;
XrSpace space = XR_NULL_HANDLE;

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
  if (m_session_ != XR_NULL_HANDLE) {
    xrDestroySession(m_session_);
  }
  if (m_instance_ != XR_NULL_HANDLE) {
    xrDestroyInstance(m_instance_);
  }
}

bool CreateSpace(const std::string& space_name, XrSpace* space) {
  if (m_session_ == XR_NULL_HANDLE) {
    return false;
  }
  XrReferenceSpaceCreateInfo referenceSpaceCreateInfo{XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
  referenceSpaceCreateInfo.poseInReferenceSpace = Math::Pose::Identity();
  referenceSpaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;

  XrResult res = xrCreateReferenceSpace(m_session_, &referenceSpaceCreateInfo, space);
  std::string message = "Failed to xrCreateReferenceSpace " + space_name;
  if (!XrCheck(NULL, xrCreateReferenceSpace(m_session_, &referenceSpaceCreateInfo, space), message.c_str())) {
    return false;
  }
  return true;
}

// https://github.com/deepmirrorinc/MobiliService/blob/39b9c7278d81482d4d6013b6ae87a3ace90e4b69/test/jni/car_pose.cc#L173
// Initialize OpenXR
extern "C" JNIEXPORT jlong JNICALL
Java_com_mobili_usbcamera_view_OpenXRInterface_initializeOpenXR(JNIEnv* env, jobject obj, jobject activity, jobject context) {
  if (xrGetInstanceProcAddr(NULL, "xrInitializeLoaderKHR", reinterpret_cast<PFN_xrVoidFunction*>(&xrInitializeLoaderKHR)) != XR_SUCCESS) {
    LOGI("Failed to get xrInitializeLoaderKHR.");
    return 0;
  }

  XrLoaderInitInfoAndroidKHR init_info;
  init_info.type = XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR;
  init_info.applicationVM = vm_;
  init_info.applicationContext = context;
  if (xrInitializeLoaderKHR(reinterpret_cast<XrLoaderInitInfoBaseHeaderKHR*>(&init_info)) != XR_SUCCESS) {
    LOGI("Failed to xrInitializeLoaderKHR.");
    return 0;
  }

  if (!PrintAPILayer()) {
    return 0;
  }
  if (!PrintExt()) {
    return 0;
  }

  XrInstanceCreateInfoAndroidKHR androidCreateInfo = {XR_TYPE_INSTANCE_CREATE_INFO_ANDROID_KHR};
  androidCreateInfo.applicationVM = vm_; // Set Java VM
  androidCreateInfo.applicationActivity = activity; // Set Java activity

  XrInstanceCreateInfo createInfo = {XR_TYPE_INSTANCE_CREATE_INFO};
  createInfo.next = &androidCreateInfo; // Attach the Android-specific struct

  strcpy(createInfo.applicationInfo.applicationName, "Mobili OpenXR App");
  createInfo.applicationInfo.applicationVersion = 1;
  strcpy(createInfo.applicationInfo.engineName, "MobiliEngine");
  createInfo.applicationInfo.engineVersion = 1;

  // Set the correct API version
  // Current version is 1.1.x, but hello_xr only requires 1.0.x
  createInfo.applicationInfo.apiVersion = XR_API_VERSION_1_0;

  // Enable necessary extensions
  const char *enabledExtensions[] = {XR_KHR_ANDROID_CREATE_INSTANCE_EXTENSION_NAME};
  createInfo.enabledExtensionCount = 1;
  createInfo.enabledExtensionNames = enabledExtensions;

  // Attempt to create the OpenXR m_instance_
  if (!XrCheck(NULL, xrCreateInstance(&createInfo, &m_instance_), "Failed to xrCreateInstance")) {
    return 0;
  }

  // 2. Get System ID
  XrSystemGetInfo systemInfo = {XR_TYPE_SYSTEM_GET_INFO};
  systemInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;

  XrSystemId xrSystemId;
  if (!XrCheck(NULL, xrGetSystem(m_instance_, &systemInfo, &xrSystemId), "Failed to xrGetSystem")) {
    return 0;
  }

  // 3. Create Session
  // Assume xrInstance and xrSystemId are valid and initialized.
  // XrGraphicsBindingOpenGLESAndroidKHR graphicsBinding = {XR_TYPE_GRAPHICS_BINDING_OPENGL_ES_ANDROID_KHR};
  // graphicsBinding.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  // graphicsBinding.context = eglGetCurrentContext();

  XrSessionCreateInfo sessionCreateInfo = {XR_TYPE_SESSION_CREATE_INFO};
  sessionCreateInfo.next = &graphicsBinding; // Provide the graphics binding
  sessionCreateInfo.systemId = xrSystemId;

  if (!XrCheck(NULL, xrCreateSession(m_instance_, &sessionCreateInfo, &m_session_), "Failed to xrCreateSession")) {
    return 0;
  }

  return reinterpret_cast<jlong>(m_session_);
}

// Retrieve pose data
extern "C" JNIEXPORT jfloatArray JNICALL
Java_com_mobili_usbcamera_view_OpenXRInterface_getDevicePose(JNIEnv* env, jobject obj) {
    if (m_instance_ == XR_NULL_HANDLE || m_session_ == XR_NULL_HANDLE || space == XR_NULL_HANDLE) {
        LOGE("Error: OpenXR not initialized properly.");
        return env->NewFloatArray(0);
    }

    // Wait for a frame to get the current time
    XrFrameState frameState = { XR_TYPE_FRAME_STATE };
    XrFrameWaitInfo frameWaitInfo = { XR_TYPE_FRAME_WAIT_INFO };
    XrResult result = xrWaitFrame(m_session_, &frameWaitInfo, &frameState);
    // logXrError(result, "xrWaitFrame");
    if (result != XR_SUCCESS) return env->NewFloatArray(0);

    // Use frameState.predictedDisplayTime as the current time
    XrSpaceLocation spaceLocation = { XR_TYPE_SPACE_LOCATION };
    result = xrLocateSpace(space, space, frameState.predictedDisplayTime, &spaceLocation);
    // logXrError(result, "xrLocateSpace");
    if (result != XR_SUCCESS) return env->NewFloatArray(0);

    // Prepare result
    jfloatArray resultArray = env->NewFloatArray(7);
    jfloat poseData[7] = {
        spaceLocation.pose.orientation.x,
        spaceLocation.pose.orientation.y,
        spaceLocation.pose.orientation.z,
        spaceLocation.pose.orientation.w,
        spaceLocation.pose.position.x,
        spaceLocation.pose.position.y,
        spaceLocation.pose.position.z
    };
    env->SetFloatArrayRegion(resultArray, 0, 7, poseData);
    return resultArray;
}
