include(ExternalProject)

if(NUP_USE_GLFW)
    set(TMP_CMAKE_ARGS "")
    list(APPEND TMP_CMAKE_ARGS "-DCMAKE_BUILD_TYPE=Release")
    list(APPEND TMP_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/external_deps")
    list(APPEND TMP_CMAKE_ARGS "-DBUILD_SHARED_LIBS=OFF")
    list(APPEND TMP_CMAKE_ARGS "-DGLFW_BUILD_TESTS=OFF")
    list(APPEND TMP_CMAKE_ARGS "-DGLFW_BUILD_EXAMPLES=OFF")
    list(APPEND TMP_CMAKE_ARGS "-DGLFW_BUILD_DOCS=OFF")

    ExternalProject_Add(external_glfw3
        # GIT_REPOSITORY  https://github.com/glfw/glfw
        GIT_REPOSITORY  https://gitee.com/scu319hy/glfw
        GIT_TAG         3.3.8
        GIT_PROGRESS    TRUE
        CMAKE_ARGS      ${TMP_CMAKE_ARGS}
    )
endif()

