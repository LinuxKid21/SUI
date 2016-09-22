cmake_minimum_required(VERSION 3.5.2)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)
set(EXAMPLES HelloWorld GridView BoxLayout Widgets InititiveTracker Animation Lua TextArea)

# most examples use the same code so why not make it into a function?
function(add_generic_example name)
    project(${name})

    file(GLOB SOURCES "./${name}/*.cpp")

    add_executable(${name} ${SOURCES})

    include_directories(${name} ../Headers)
    target_link_libraries(${name} sui ${SUI_LIBS})
endfunction(add_generic_example)


foreach(example ${EXAMPLES})
    add_generic_example(${example})
endforeach()
