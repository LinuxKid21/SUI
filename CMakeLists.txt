cmake_minimum_required(VERSION 3.5.2)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)
project(sui_static)

file(GLOB SOURCES "Source/SUI/*.cpp")
file(GLOB HEADERS "Headers/SUI/*.hpp")

add_library(sui_static STATIC ${SOURCES})


include_directories(./Headers)

if(UNIX)
target_link_libraries(sui_static
	sfml-graphics
	sfml-audio
	sfml-window
	sfml-system
	readline
	${CMAKE_CURRENT_SOURCE_DIR}/Lua/linux/liblua.so
	dl)
elseif(WIN32)
	include_directories(./SFML)
	target_link_libraries(sui_static
		${CMAKE_CURRENT_SOURCE_DIR}/SFML/Libs/libsfml-graphics.a
		${CMAKE_CURRENT_SOURCE_DIR}/SFML/Libs/libsfml-audio.a
		${CMAKE_CURRENT_SOURCE_DIR}/SFML/Libs/libsfml-window.a
		${CMAKE_CURRENT_SOURCE_DIR}/SFML/Libs/libsfml-system.a)
else()
	message( FATAL_ERROR "unknown operating system")
endif()
install(
	TARGETS
		sui_static
	DESTINATION
		${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Examples)
