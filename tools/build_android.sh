#!/bin/bash



cmake -S . -DTARGET_PLATFORM=Android -DANDROID_ABI=armeabi-v7a -DCMAKE_TOOLCHAIN_FILE=/opt/android-sdk/ndk/21.2.6472646/build/cmake/android.toolchain.cmake -B build-android-32 
cmake --build build-android-32 -j 4

cmake -S . -DTARGET_PLATFORM=Android -DANDROID_ABI=armeabi-v7a -DCMAKE_TOOLCHAIN_FILE=/opt/android-sdk/ndk/21.2.6472646/build/cmake/android.toolchain.cmake -B build-android-64 
cmake --build build-android-64 -j 4

pushd .;
cd /home/niu2x/project/nup/nup/src/android;
./gradlew :app:assembleDebug
popd;