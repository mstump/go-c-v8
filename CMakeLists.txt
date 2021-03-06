CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
set(PROJECT_NAME_STR gocv8)
project(${PROJECT_NAME_STR} C CXX)

#-------------------------------------------------------------------------------
# The version number
#-------------------------------------------------------------------------------
set (libgocv8_VERSION_MAJOR 0)
set (libgocv8_VERSION_MINOR 5)
set (libgocv8_VERSION_STRING ${libgocv8_VERSION_MAJOR}.${libgocv8_VERSION_MINOR})

#-------------------------------------------------------------------------------
# setup deps
#-------------------------------------------------------------------------------
set(EXT_PREFIX ${PROJECT_SOURCE_DIR}/ext)
set(CMAKE_MODULE_PATH ${EXT_PREFIX})

# OS specific lib stuff
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_STATIC_RUNTIME ON)
else()
  set(Boost_USE_STATIC_LIBS OFF)
  set(Boost_USE_STATIC_RUNTIME OFF)
endif()

# Boost
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.47.0 )
set(INCLUDES ${INCLUDES} ${Boost_INCLUDE_DIRS} )
set(LIBS ${LIBS} ${Boost_LIBRARIES} )

# v8
set(V8_INCLUDE_NAMES v8.h v8-debug.h v8-profiler.h v8stdint.h)
set(V8_LIBRARY_NAMES v8)

find_path(V8_INCLUDE_DIRS NAMES ${V8_INCLUDE_NAMES} HINTS /usr/include /usr/local/include)
find_library(V8_LIBRARIES NAMES ${V8_LIBRARY_NAMES} HINTS /usr/include /usr/local/include)

if(V8_INCLUDE_DIRS AND V8_LIBRARIES)
  set(V8_FOUND TRUE)
  set(INCLUDES ${INCLUDES} ${V8_INCLUDE_DIR} )
  set(LIBS ${LIBS} ${V8_LIBRARIES} )
  message(STATUS "Found V8: ${V8_LIBRARIES}")
else(V8_INCLUDE_DIRS AND V8_LIBRARIES)
  message(FATAL_ERROR "Could not find V8 library.")
endif(V8_INCLUDE_DIRS AND V8_LIBRARIES)


#-------------------------------------------------------------------------------
# build missing deps, if applicable
#-------------------------------------------------------------------------------
add_custom_target (AppDependencies ALL
  DEPENDS ${APP_DEPENDENCIES}
  COMMENT "Installing any missing dependencies.")

if(DEFINED ENV{VERBOSE})
  message(STATUS "All Variables")
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
endif()

#-------------------------------------------------------------------------------
# libgocv8 static, shared and executable
#-------------------------------------------------------------------------------
set(PROJECT_LIB_NAME ${PROJECT_NAME_STR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build/bin/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories(${INCLUDES} ${PROJECT_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/src)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_LIB_NAME} SHARED ${SRC_FILES})
add_library(${PROJECT_LIB_NAME}-static STATIC ${SRC_FILES})

target_link_libraries(${PROJECT_LIB_NAME} ${LIBS})
SET_TARGET_PROPERTIES(${PROJECT_LIB_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_LIB_NAME})
SET_TARGET_PROPERTIES(${PROJECT_LIB_NAME} PROPERTIES VERSION ${libgocv8_VERSION_MAJOR}.${libgocv8_VERSION_MINOR} SOVERSION ${libgocv8_VERSION_MAJOR})

#-------------------------------------------------------------------------------
# Set compiler flags
#-------------------------------------------------------------------------------
set(PROJECT_COMPILER_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -pedantic -Wextra -Werror -Wno-long-long")

set_property(
  TARGET ${PROJECT_LIB_NAME}
  APPEND PROPERTY COMPILE_FLAGS ${PROJECT_COMPILER_FLAGS})

set_property(
  TARGET ${PROJECT_LIB_NAME}-static
  APPEND PROPERTY COMPILE_FLAGS ${PROJECT_COMPILER_FLAGS})

#-------------------------------------------------------------------------------
# install target
#-------------------------------------------------------------------------------
FILE(GLOB INSTALL_HEADERS "${PROJECT_INCLUDE_DIR}/gocv8/*.h")
INSTALL(FILES ${INSTALL_HEADERS} DESTINATION include/gocv8)

INSTALL(TARGETS ${PROJECT_LIB_NAME}-static ${PROJECT_LIB_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

#-------------------------------------------------------------------------------
# uninstall target
#-------------------------------------------------------------------------------
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

#-------------------------------------------------------------------------------
# unit tests
#-------------------------------------------------------------------------------
enable_testing()
add_subdirectory(test)

#-------------------------------------------------------------------------------
# the demo program
#-------------------------------------------------------------------------------
# add_subdirectory(src/demo EXCLUDE_FROM_ALL)
