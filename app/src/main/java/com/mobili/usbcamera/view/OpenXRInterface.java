// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2021 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

package com.mobili.usbcamera.view;

import android.content.Context;
import android.app.Activity;
import android.content.res.AssetManager;
import android.view.Surface;

public class OpenXRInterface {
    static {
        System.loadLibrary("openxr_interface"); // Load your native library
    }

    public native long initialize(Activity activity, Context context);
    public native float[] getDevicePose();
    public native void passImage(long timestamp, int image_width, int image_height, byte[] byteArray);
}
