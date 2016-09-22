cmake_minimum_required(VERSION 3.5.2)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)


if("${BUILD_FILE}" STREQUAL "")
	message(FATAL_ERROR "BUILD_FILE must be set. see Build.txt step 2.")
endif()
include("${BUILD_FILE}")


include("ConfigOptions.cmake")



project(sui)

file(GLOB SOURCES "Source/SUI/*.cpp")
file(GLOB HEADERS "Headers/SUI/*.hpp")

add_library(sui ${SUI_BUILD_TYPE} ${SOURCES})

include_directories(./Headers)
target_link_libraries(sui ${SUI_LIBS})

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin)



if(BUILD_EXAMPLES)
	# handle dlls examples need
	#if(WIN32 AND SUI_SHARED)
		
	#endif()
	
	file(GLOB EXAMPLE_ASSETS "${CMAKE_SOURCE_DIR}/Examples/Assets/*")
	add_custom_command(TARGET sui POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EXAMPLE_ASSETS} ${CMAKE_SOURCE_DIR}/Bin)
	
	add_subdirectory(./Examples)
endif()
