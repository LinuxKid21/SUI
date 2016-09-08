cmake_minimum_required(VERSION 3.5.2)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)
project(sui_static)

file(GLOB SOURCES "Source/SUI/*.cpp")
file(GLOB HEADERS "Headers/SUI/*.hpp")

add_library(sui_static STATIC ${SOURCES})

include_directories(./Headers)
target_link_libraries(sui_static
	sfml-graphics
	sfml-audio
	sfml-window
	sfml-system)

install(
	TARGETS
		sui_static
	DESTINATION
		${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Examples)
