# Install script for directory: /user_data/CodeProjects/SUI/Examples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/HelloWorld" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/HelloWorld")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/HelloWorld"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/user_data/CodeProjects/SUI/Examples/Binaries/HelloWorld")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/user_data/CodeProjects/SUI/Examples/Binaries" TYPE EXECUTABLE FILES "/user_data/CodeProjects/SUI/Build/Examples/HelloWorld")
  if(EXISTS "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/HelloWorld" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/HelloWorld")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/HelloWorld")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/GridView" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/GridView")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/GridView"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/user_data/CodeProjects/SUI/Examples/Binaries/GridView")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/user_data/CodeProjects/SUI/Examples/Binaries" TYPE EXECUTABLE FILES "/user_data/CodeProjects/SUI/Build/Examples/GridView")
  if(EXISTS "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/GridView" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/GridView")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/GridView")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/BoxLayout" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/BoxLayout")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/BoxLayout"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/user_data/CodeProjects/SUI/Examples/Binaries/BoxLayout")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/user_data/CodeProjects/SUI/Examples/Binaries" TYPE EXECUTABLE FILES "/user_data/CodeProjects/SUI/Build/Examples/BoxLayout")
  if(EXISTS "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/BoxLayout" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/BoxLayout")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/BoxLayout")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/Widgets" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/Widgets")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/Widgets"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/user_data/CodeProjects/SUI/Examples/Binaries/Widgets")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/user_data/CodeProjects/SUI/Examples/Binaries" TYPE EXECUTABLE FILES "/user_data/CodeProjects/SUI/Build/Examples/Widgets")
  if(EXISTS "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/Widgets" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/Widgets")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/Widgets")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/InititiveTracker" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/InititiveTracker")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/InititiveTracker"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/user_data/CodeProjects/SUI/Examples/Binaries/InititiveTracker")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/user_data/CodeProjects/SUI/Examples/Binaries" TYPE EXECUTABLE FILES "/user_data/CodeProjects/SUI/Build/Examples/InititiveTracker")
  if(EXISTS "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/InititiveTracker" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/InititiveTracker")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/user_data/CodeProjects/SUI/Examples/Binaries/InititiveTracker")
    endif()
  endif()
endif()

