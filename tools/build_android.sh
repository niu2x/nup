#!/bin/bash

# pushd .;
# cd /home/niu2x/project/nup/nup/src/android;
# ./gradlew :app:assembleDebug
# popd;


cmake -S . -DTARGET_PLATFORM=Android -DCMAKE_TOOLCHAIN_FILE=/opt/android-sdk/ndk/21.2.6472646/build/cmake/android.toolchain.cmake -B build-android 
cmake --build build-android -j 4