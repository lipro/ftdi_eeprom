project(ftdi_eeprom)
set(MAJOR_VERSION 0)
set(MINOR_VERSION 4)
set(VERSION_STRING ${MAJOR_VERSION}.${MINOR_VERSION})
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

# Package information
set(CPACK_PACKAGE_VERSION              ${VERSION_STRING})
set(CPACK_PACKAGE_CONTACT              "http://www.intra2net.com/en/developer/libftdi/mailinglist.php")
set(CPACK_PACKAGE_DESCRIPTION          "ftdi eeprom tool.")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY  ${CPACK_PACKAGE_DESCRIPTION}
                                       )
# Package settings
if(${UNIX})
  set(CPACK_GENERATOR                    "DEB;RPM")
  set(CPACK_CMAKE_GENERATOR              "Unix Makefiles")
  set(CPACK_PACKAGE_NAME                 ${PROJECT_NAME})
  set(CPACK_PACKAGE_FILE_NAME            ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})
endif(${UNIX})

if(${WIN32})
  set(CPACK_GENERATOR                    "NSIS")
  set(CPACK_CMAKE_GENERATOR              "MinGW Makefiles")
  set(CPACK_PACKAGE_NAME                 "${PROJECT_NAME}")
  set(CPACK_PACKAGE_VENDOR               "")
  set(CPACK_PACKAGE_INSTALL_DIRECTORY    "ftdi_eeprom")
  set(CPACK_PACKAGE_FILE_NAME            "${PROJECT_NAME}-${VERSION_STRING}-win32")
  set(CPACK_NSIS_DISPLAY_NAME            "ftdi_eeprom")
  set(CPACK_NSIS_MODIFY_PATH             "ON")
endif(${WIN32})

set(CPACK_SOURCE_GENERATOR             TGZ)
set(CPACK_SOURCE_IGNORE_FILES          "\\\\.git")
set(CPACK_SOURCE_PACKAGE_FILE_NAME     ${CPACK_PACKAGE_FILE_NAME})

set(CPACK_RESOURCE_FILE_LICENSE        ${CMAKE_SOURCE_DIR}/COPYING)

# Subdirectories
if(${UNIX})
  set(CPACK_SET_DESTDIR "ON")
endif(${UNIX})

add_subdirectory(src)

include(CPack)
