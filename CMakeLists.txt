cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)
project(NIU2X_NUP VERSION 0.1.0 LANGUAGES CXX C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(ExternalDeps)
include(warning_as_error)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(nup)