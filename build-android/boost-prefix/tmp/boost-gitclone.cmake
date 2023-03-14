
if(NOT "/home/niu2x/project/nup/build-android/boost-prefix/src/boost-stamp/boost-gitinfo.txt" IS_NEWER_THAN "/home/niu2x/project/nup/build-android/boost-prefix/src/boost-stamp/boost-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/home/niu2x/project/nup/build-android/boost-prefix/src/boost-stamp/boost-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E remove_directory "/home/niu2x/project/nup/build-android/boost-prefix/src/boost"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/niu2x/project/nup/build-android/boost-prefix/src/boost'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout --progress "https://gitee.com/add358/boost" "boost"
    WORKING_DIRECTORY "/home/niu2x/project/nup/build-android/boost-prefix/src"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://gitee.com/add358/boost'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout 1.71.0 --
  WORKING_DIRECTORY "/home/niu2x/project/nup/build-android/boost-prefix/src/boost"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '1.71.0'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/home/niu2x/project/nup/build-android/boost-prefix/src/boost"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/niu2x/project/nup/build-android/boost-prefix/src/boost'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/home/niu2x/project/nup/build-android/boost-prefix/src/boost-stamp/boost-gitinfo.txt"
    "/home/niu2x/project/nup/build-android/boost-prefix/src/boost-stamp/boost-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/niu2x/project/nup/build-android/boost-prefix/src/boost-stamp/boost-gitclone-lastrun.txt'")
endif()

