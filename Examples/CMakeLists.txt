set(CMAKE_BUILD_TYPE Debug)
set(EXAMPLE_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Binaries)
set(EXAMPLES HelloWorld GridView BoxLayout Widgets InititiveTracker Animation Lua TextArea)

# most examples use the same code so why not make it into a function?
function(add_generic_example name)
    project(${name})

    file(GLOB SOURCES "./${name}/*.cpp")

    add_executable(${name} ${SOURCES})

    target_link_libraries(${name}
    	sui_static)

	if(UNIX)
	target_link_libraries(${name}
		sfml-graphics
		sfml-audio
		sfml-window
		sfml-system)
	elseif(WIN32)
		include_directories(./SFML)
		target_link_libraries(${name}
			${CMAKE_CURRENT_SOURCE_DIR}/../SFML/Libs/libsfml-graphics.a
			${CMAKE_CURRENT_SOURCE_DIR}/../SFML/Libs/libsfml-audio.a
			${CMAKE_CURRENT_SOURCE_DIR}/../SFML/Libs/libsfml-window.a
			${CMAKE_CURRENT_SOURCE_DIR}/../SFML/Libs/libsfml-system.a)
	else()
		message( FATAL_ERROR "unknown operating system")
	endif()
    install(
    	TARGETS
    		${name}
    	DESTINATION
    		${EXAMPLE_BIN_DIR})
endfunction(add_generic_example)


foreach(example ${EXAMPLES})
    add_generic_example(${example})
endforeach()
