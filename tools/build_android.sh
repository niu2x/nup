#!/bin/bash

source /usr/local/share/nxsh/nxsh.sh;

cmake -S . -DTARGET_PLATFORM=Android -DANDROID_ABI=armeabi-v7a -DCMAKE_TOOLCHAIN_FILE=/opt/android-sdk/ndk/21.2.6472646/build/cmake/android.toolchain.cmake -B build-android-32 
cmake --build build-android-32 -j 4

cmake -S . -DTARGET_PLATFORM=Android -DANDROID_ABI=arm64-v8a -DCMAKE_TOOLCHAIN_FILE=/opt/android-sdk/ndk/21.2.6472646/build/cmake/android.toolchain.cmake -B build-android-64 
cmake --build build-android-64 -j 4

mkdir -p nup/src/android/app/src/main/jniLibs

cp build-android-32/nup/libnup.so nup/src/android/app/src/main/jniLibs/armeabi-v7a
cp build-android-64/nup/libnup.so nup/src/android/app/src/main/jniLibs/arm64-v8a

pushd .;
cd ./nup/src/android;
./gradlew :app:assembleDebug
popd;