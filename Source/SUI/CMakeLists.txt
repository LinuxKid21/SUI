cmake_minimum_required(VERSION 3.5.2)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)
project(sui_static)

file(GLOB SOURCES "Source/*.cpp")
file(GLOB FONT_FILES "./*.ttf")

add_library(sui_static STATIC ${SOURCES})

# add_executable(exec ${SOURCES})

include_directories(./Source ./Source/Actors /usr/include)
link_directories(/usr/lib)
target_link_libraries(sui_static
	pthread
	sfml-graphics
	sfml-audio
	sfml-window
	sfml-system
	readline
	dl)
include_directories(/usr/include)

set(FONTS)

install(
	TARGETS
		sui_static
	DESTINATION
		${CMAKE_CURRENT_SOURCE_DIR}/Tests)
install(
	FILES
		${FONT_FILES}
	DESTINATION
		${CMAKE_CURRENT_SOURCE_DIR}/Tests)
