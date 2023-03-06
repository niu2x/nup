build:
	cmake -S . -B build \
		-DBUILD_SHARED_LIBS=ON \
		-DOpenSSL_ROOT=/home/niu2x/project/openssl/dist/usr/local/ 
	cmake --build build

change-version:
	find . -name CMakeLists.txt|while read item; do \
		echo $$item; \
		sed -i "s/ VERSION [0-9]\+\.[0-9]\+\.[0-9]\+/ VERSION $(version)/" $$item; \
	done;

clean: 
	git clean -fdx

.PHONY: build change-version clean