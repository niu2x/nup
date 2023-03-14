
if(NOT "/home/niu2x/project/nup/build-android/glfw3-prefix/src/glfw3-stamp/glfw3-gitinfo.txt" IS_NEWER_THAN "/home/niu2x/project/nup/build-android/glfw3-prefix/src/glfw3-stamp/glfw3-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/home/niu2x/project/nup/build-android/glfw3-prefix/src/glfw3-stamp/glfw3-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E remove_directory "/home/niu2x/project/nup/build-android/glfw3-prefix/src/glfw3"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/niu2x/project/nup/build-android/glfw3-prefix/src/glfw3'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout --progress "https://github.com/glfw/glfw" "glfw3"
    WORKING_DIRECTORY "/home/niu2x/project/nup/build-android/glfw3-prefix/src"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/glfw/glfw'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout 3.3.8 --
  WORKING_DIRECTORY "/home/niu2x/project/nup/build-android/glfw3-prefix/src/glfw3"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '3.3.8'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/home/niu2x/project/nup/build-android/glfw3-prefix/src/glfw3"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/niu2x/project/nup/build-android/glfw3-prefix/src/glfw3'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/home/niu2x/project/nup/build-android/glfw3-prefix/src/glfw3-stamp/glfw3-gitinfo.txt"
    "/home/niu2x/project/nup/build-android/glfw3-prefix/src/glfw3-stamp/glfw3-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/niu2x/project/nup/build-android/glfw3-prefix/src/glfw3-stamp/glfw3-gitclone-lastrun.txt'")
endif()

