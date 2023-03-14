set(LIB_NAME nup)

add_executable(${LIB_NAME} "")
warning_as_error_enable(${LIB_NAME})

include(GenerateExportHeader)
generate_export_header(${LIB_NAME}
	EXPORT_FILE_NAME include/${LIB_NAME}/api.h
	EXPORT_MACRO_NAME NUP_API
	DEPRECATED_MACRO_NAME DEPRECATED_API
	INCLUDE_GUARD_NAME NUP_API_H
)

target_compile_definitions(${LIB_NAME} PRIVATE ${LIB_NAME}_EXPORTS)

set_target_properties(${LIB_NAME} PROPERTIES
	C_VISIBILITY_PRESET hidden
	CXX_VISIBILITY_PRESET hidden
	VISIBILITY_INLINES_HIDDEN ON
)

list(APPEND SOURCE_DIRECTORIES "src/*.cpp")

if(TARGET_PLATFORM STREQUAL "Linux")
	list(APPEND SOURCE_DIRECTORIES "src/linux/*.cpp")
	list(APPEND SOURCE_DIRECTORIES "src/glfw/*.cpp")
endif()

file(GLOB source 
	${SOURCE_DIRECTORIES}
)


target_sources(${LIB_NAME} PRIVATE
	${source}
)

target_include_directories(${LIB_NAME} PUBLIC
	src/
	${CMAKE_CURRENT_BINARY_DIR}/include
)

# find_package(OpenSSL 3.0 REQUIRED)
	
set(glfw3_DIR "${PROJECT_BINARY_DIR}/external_deps/lib/cmake/glfw3" CACHE PATH "" FORCE)
find_package(glfw3 REQUIRED)

set(glfw3_DIR "${PROJECT_BINARY_DIR}/boost-prefix/src/boost/stage/lib/cmake/boost_timer-1.81.0" CACHE PATH "" FORCE)
find_package(Boost REQUIRED)


target_link_libraries(${LIB_NAME} PRIVATE
	# OpenSSL::Crypto
	# OpenSSL::SSL
	glfw
	Boost::headers
)

