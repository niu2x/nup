#!/bin/bash

pushd .;
cd /home/niu2x/project/nup/nup/src/android;
./gradlew :app:assembleDebug
popd;