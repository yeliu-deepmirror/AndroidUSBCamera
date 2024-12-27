AndroidUSBCamera
============

AndroidUSBCamera is developed based on the [saki4510t/UVCCamera](https://github.com/saki4510t/UVCCamera), the project of USB Camera (UVC equipment) and the use of video data acquisition are highly packaged, and it can help developers using USB Camera devices easily by a few simple APIs. By using AndroidUSBCamera,you can detect and connect to a USB Camera simply.And you also can use it to realize taking picture,recording mp4,switching resolutions ,getting h.264/aac/yuv(nv21) stream and setting  camera's contrast or brightness,supporting 480P、720P、1080P and higher,etc.supporting overlay and record device's mic.   

Supporting Android 5.0,6.0,7.0,8.0,9.0,10.0


Objective
------------------------------

Most VR devices does not allow access to the internal RGB camera, which limit our power.
In order to develop a large space VR experience, we need RGB camera to process space localization, which lead us to add a USB camera.


What this project do :
* Open USB camera, pass the image to JNI native layer for processing.
* Open an grpc server to allow streaming the images.
* Localize the image in space (currently using marker images).
* Receive request from any XR system (through websocket), fusion the two trajectory, estimation the transformation between the two coordinate systems (XR coordinate system & large space coordinate system)

[image](artifacts/quest_usb_cam.jpg)
