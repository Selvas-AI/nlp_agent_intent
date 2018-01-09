#####################################################################
# Set Dependency Library
#####################################################################
set(PROJECT_ROOT_DIR "${CMAKE_SOURCE_DIR}")
set(BUILD_POLICY_DIR ${PROJECT_ROOT_DIR}/build-policy)

include(${CMAKE_SOURCE_DIR}/build-policy/toolchain_settings.cmake)
cmake_minimum_required(VERSION 2.8)
project(fasttext)
#####################################################################

set(REQUIRED_PYTHON_VERSION 3.5)
set(Python_ADDITIONAL_VERSIONS 3.5)
if(WIN32)
  find_program(PYTHON_EXECUTABLE NAMES python PATHS "C:\\Python27" NO_DEFAULT_PATH)
  if (PYTHON_EXECUTABLE)
    set(PYTHON_VERSION_STRING "${REQUIRED_PYTHON_VERSION}")
  else()
    find_package(PythonInterp ${REQUIRED_PYTHON_VERSION} REQUIRED)
  endif()
else()
    find_package(PythonInterp ${REQUIRED_PYTHON_VERSION} REQUIRED)
endif()
if(PYTHON_EXECUTABLE)
  message(STATUS "PYTHON_VERSION_STRING=${PYTHON_VERSION_STRING}")
  message(STATUS "PYTHON_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE}")
endif()  

get_current_date(BUILD_DATE)
get_platform_name(PLATFORM TARGET_ABI)

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_IMPORT_LIBRARY_PREFIX "")

set(CURRENT_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(TARGET_PLATFORM_NAME ${PLATFORM}_${TARGET_ABI})
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

#####################################################################
option(BUILD_PYTHON "Build Python Interface" OFF)

include(${PROJECT_ROOT_DIR}/config.cmake)

#####################################################################
# Setting build options
include(${BUILD_POLICY_DIR}/build_option.cmake)

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

#####################################################################
# Build Target
add_subdirectory(src)
add_subdirectory(tool)
