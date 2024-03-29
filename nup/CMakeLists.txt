set(LIB_NAME nup)

if(TARGET_PLATFORM STREQUAL "Linux")
	add_executable(${LIB_NAME} "")
else()
	add_library(${LIB_NAME} SHARED "")
	include(GenerateExportHeader)
	generate_export_header(${LIB_NAME}
		EXPORT_FILE_NAME include/${LIB_NAME}/api.h
		EXPORT_MACRO_NAME NUP_API
		DEPRECATED_MACRO_NAME DEPRECATED_API
		INCLUDE_GUARD_NAME NUP_API_H
	)
endif()

warning_as_error_enable(${LIB_NAME})

# target_compile_definitions(${LIB_NAME} PRIVATE ${LIB_NAME}_EXPORTS)

set_target_properties(${LIB_NAME} PROPERTIES
	C_VISIBILITY_PRESET hidden
	CXX_VISIBILITY_PRESET hidden
	VISIBILITY_INLINES_HIDDEN ON
)

list(APPEND SOURCE_DIRECTORIES "src/*.cpp")
set(LIBS "")


if(NUP_USE_GLFW)
	list(APPEND SOURCE_DIRECTORIES "src/glfw/*.cpp")

	set(glfw3_DIR "${PROJECT_BINARY_DIR}/external_deps/lib/cmake/glfw3" CACHE PATH "" FORCE)
	find_package(glfw3 REQUIRED)
	list(APPEND LIBS glfw)
endif()

if(TARGET_PLATFORM STREQUAL "Linux")
	list(APPEND SOURCE_DIRECTORIES "src/linux/*.cpp")
endif()

if(TARGET_PLATFORM STREQUAL "Android")
	list(APPEND SOURCE_DIRECTORIES "src/android/cpp/*.cpp")
	list(APPEND LIBS "EGL")
endif()


file(GLOB source 
	${SOURCE_DIRECTORIES}
)

target_sources(${LIB_NAME} PRIVATE
	${source}
)

target_include_directories(${LIB_NAME} PRIVATE
	src/
	${CMAKE_CURRENT_BINARY_DIR}/include
	../3rd/
)

target_link_libraries(${LIB_NAME} PRIVATE
	${LIBS}
	
)

