build-linux:
	cmake -S . -DTARGET_PLATFORM=Linux -B build
	cmake --build build

build-android:
	cmake -S . -DTARGET_PLATFORM=Android -DCMAKE_TOOLChAIN_FILE=/opt/android-sdk/ndk/21.2.6472646/build/cmake/android.toolchain.cmake -B build 
	cmake --build build

change-version:
	find . -name CMakeLists.txt|while read item; do \
		echo $$item; \
		sed -i "s/ VERSION [0-9]\+\.[0-9]\+\.[0-9]\+/ VERSION $(version)/" $$item; \
	done;

clean: 
	git clean -fdx

.PHONY: build change-version clean