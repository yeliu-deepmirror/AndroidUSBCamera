package com.mobili.usbcamera.utils;

import android.content.Intent;
import android.service.vr.VrListenerService;
import android.util.Log;

public class MyVrListenerService extends VrListenerService {

    private static final String TAG = "[MOBILI] MyVrListenerService";

    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG, "VR Listener Service Created");
        // 在服务启动时执行你的逻辑
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "VR Listener Service Destroyed");
        // 在服务销毁时清理资源
    }

    // @Override
    // public void onCurrentVrModeChanged(boolean enabled) {
    //     super.onCurrentVrModeChanged(enabled);
    //     Log.d(TAG, "VR Mode Changed: " + enabled);
    //
    //     if (enabled) {
    //         // 启动已有的 ForegroundService
    //         Intent serviceIntent = new Intent(this, MyForegroundService.class);
    //         startService(serviceIntent);
    //     } else {
    //         // 停止 ForegroundService
    //         Intent serviceIntent = new Intent(this, MyForegroundService.class);
    //         stopService(serviceIntent);
    //     }
    // }
}
