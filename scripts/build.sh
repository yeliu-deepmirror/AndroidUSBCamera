#!/usr/bin/env bash
set -e

APP_PATH=/AndroidUSBCamera/app/build/outputs/apk/debug/app-debug.apk
./gradlew assembleDebug

mkdir -p output
cp ${APP_PATH} output/app-debug.apk

if adb get-state 1>/dev/null 2>&1; then
  echo "Device found"
  adb install ${APP_PATH}
else
  echo "Device not found"
fi
