set(CMAKE_BUILD_TYPE Debug)
set(EXAMPLE_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Binaries)
set(EXAMPLES HelloWorld GridView BoxLayout Widgets InititiveTracker)

# most examples use the same code so why not make it into a function?
function(add_generic_example name)
    project(${name})

    file(GLOB SOURCES "./${name}/*.cpp")

    add_executable(${name} ${SOURCES})

    target_link_libraries(${name}
    	sfml-graphics
    	sfml-audio
    	sfml-window
    	sfml-system
    	sui_static)

    install(
    	TARGETS
    		${name}
    	DESTINATION
    		${EXAMPLE_BIN_DIR})
endfunction(add_generic_example)


foreach(example ${EXAMPLES})
    add_generic_example(${example})
endforeach()
