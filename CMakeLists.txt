project(ftdi_eeprom)
set(MAJOR_VERSION 0)
set(MINOR_VERSION 4)
set(PATCH_VERSION 0)
set(VERSION_STRING ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION})

# CMake
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
   set(CMAKE_BUILD_TYPE Debug)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set(CMAKE_COLOR_MAKEFILE ON)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# Debug build
message("-- Build type: ${CMAKE_BUILD_TYPE}")
if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
   add_definitions(-DDEBUG)
endif(${CMAKE_BUILD_TYPE} STREQUAL Debug)

include(FindPkgConfig)
pkg_check_modules(Confuse REQUIRED libconfuse)
INCLUDE_DIRECTORIES(${Confuse_INCLUDE_DIRS})
LINK_DIRECTORIES(${Confuse_LIBRARY_DIRS})
SET(libs ${libs} ${Confuse_LIBRARIES})

pkg_check_modules(FTDI REQUIRED libftdi)
INCLUDE_DIRECTORIES(${FTDI_INCLUDE_DIRS})
LINK_DIRECTORIES(${FTDI_LIBRARY_DIRS})
SET(libs ${libs} ${FTDI_LIBRARIES})

configure_file(${PROJECT_SOURCE_DIR}/config.h.cmake ${PROJECT_BINARY_DIR}/config.h)
add_definitions(-DHAVE_CONFIG_H)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

add_subdirectory(src)
