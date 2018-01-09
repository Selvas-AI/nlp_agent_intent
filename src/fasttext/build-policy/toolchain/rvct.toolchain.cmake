cmake_minimum_required( VERSION 2.6.3 )
include(CMakeForceCompiler)

macro( __INIT_VARIABLE var_name )
 set( __test_path 0 )
 foreach( __var ${ARGN} )
  if( __var STREQUAL "PATH" )
   set( __test_path 1 )
   break()
  endif()
 endforeach()
 if( __test_path AND NOT EXISTS "${${var_name}}" )
  unset( ${var_name} CACHE )
 endif()
 if( "${${var_name}}" STREQUAL "" )
  set( __values 0 )
  foreach( __var ${ARGN} )
   if( __var STREQUAL "VALUES" )
    set( __values 1 )
   elseif( NOT __var STREQUAL "PATH" )
    set( __obsolete 0 )
    if( __var MATCHES "^OBSOLETE_.*$" )
     string( REPLACE "OBSOLETE_" "" __var "${__var}" )
     set( __obsolete 1 )
    endif()
    if( __var MATCHES "^ENV_.*$" )
     string( REPLACE "ENV_" "" __var "${__var}" )
     set( __value "$ENV{${__var}}" )
    elseif( DEFINED ${__var} )
     set( __value "${${__var}}" )
    else()
     if( __values )
      set( __value "${__var}" )
     else()
      set( __value "" )
     endif()
    endif()
    if( NOT "${__value}" STREQUAL "" )
     if( __test_path )
      if( EXISTS "${__value}" )
       file( TO_CMAKE_PATH "${__value}" ${var_name} )
       if( __obsolete AND NOT _CMAKE_IN_TRY_COMPILE )
        message( WARNING "Using value of obsolete variable ${__var} as initial value for ${var_name}. Please note, that ${__var} can be completely removed in future versions of the toolchain." )
       endif()
       break()
      endif()
     else()
      set( ${var_name} "${__value}" )
       if( __obsolete AND NOT _CMAKE_IN_TRY_COMPILE )
        message( WARNING "Using value of obsolete variable ${__var} as initial value for ${var_name}. Please note, that ${__var} can be completely removed in future versions of the toolchain." )
       endif()
      break()
     endif()
    endif()
   endif()
  endforeach()
  unset( __value )
  unset( __values )
  unset( __obsolete )
 elseif( __test_path )
  file( TO_CMAKE_PATH "${${var_name}}" ${var_name} )
 endif()
 unset( __test_path )
endmacro()

SET(CMAKE_SYSTEM_NAME RVCT)
SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_SYSTEM_VERSION 1)

message("RVCT_VERSION = ${RVCT_VERSION}")
IF (NOT RVCT_VERSION)
    MESSAGE(FATAL_ERROR "Please set RVCT_VERSION")
ENDIF()
__INIT_VARIABLE(RVCT_INC PATH ENV_ARMCC${RVCT_VERSION}INC)
__INIT_VARIABLE(RVCT_LIB PATH ENV_ARMCC${RVCT_VERSION}LIB)
__INIT_VARIABLE(RVCT_BIN PATH ENV_ARMCC${RVCT_VERSION}BIN)
IF(NOT RVCT_INC OR NOT RVCT_LIB OR NOT RVCT_BIN)
  MESSAGE(FATAL_ERROR "RVDS Environment not found")
ENDIF()
IF (NOT RVCT_TARGET_CPU)
  MESSAGE(FATAL_ERROR "Please set RVCT_TARGET_CPU")
ENDIF()

# This is a hack because RVCT only exports symbols already defined in the symdefs
# file if the file already exists
SET(CMAKE_IMPORT_LIBRARY_SUFFIX ".symdefs")
SET(SYMDEFS_RAW_SUFFIX ".raw")
SET(SYMDEFS_POST_BUILD
        "rm -f <TARGET_IMPLIB>"
        "mv <TARGET_IMPLIB>${SYMDEFS_RAW_SUFFIX} <TARGET_IMPLIB>")

# Set the C/C++ compiler to use armcc
FIND_PROGRAM(ARMCC NAMES armcc PATHS ${RVCT_BIN})
CMAKE_FORCE_C_COMPILER(${ARMCC} RVCT)
CMAKE_FORCE_CXX_COMPILER(${ARMCC} RVCT)
message("ARMCC = ${ARMCC}")

# Set the archiver to armar
FIND_PROGRAM(ARMAR NAMES armar PATHS ${RVCT_BIN})
set(CMAKE_AR ${ARMAR})
message("ARMAR = ${ARMAR}")

# Set the ASM compiler to be armasm
FIND_PROGRAM(ARMASM NAMES armasm PATHS ${RVCT_BIN})
set(CMAKE_ASM_RVCT_COMPILER ${ARMASM})
message("ARMASM = ${ARMASM}")

# Set the dynamic linker to armlink
FIND_PROGRAM(ARMLINK NAMES armlink PATHS ${RVCT_BIN})
set(CMAKE_LINKER ${ARMLINK})
message("ARMLINK = ${ARMLINK}")

# Find the fromelf program
FIND_PROGRAM(ARMFROMELF NAMES fromelf PATHS ${RVCT_BIN})
set(FROMELF ${ARMFROMELF})
message("ARMFROMELF = ${ARMFROMELF}")

# Set up include flags
SET(CMAKE_INCLUDE_FLAG_C "-I")
SET(CMAKE_INCLUDE_FLAG_CXX "-I")
SET(CMAKE_INCLUDE_SYSTEM_FLAG_C "-I")
SET(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-I")

set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
set(CMAKE_SHARED_LIBRARY_SUFFIX ".a")
set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")

# Default C flags
SET(RVCT_OPTION "-c --diag_suppress=111,185,1201,1293,1300,1294 --cpu ${RVCT_TARGET_CPU}")
SET(CMAKE_C_FLAGS "${RVCT_OPTION}" CACHE INTERNAL "Thumb debug C flags" FORCE)
SET(CMAKE_CXX_FLAGS "${RVCT_OPTION}" CACHE INTERNAL "Thumb generic C++ flags" FORCE)

SET(CMAKE_C_FLAGS_DEBUG "-O0 --dwarf2 --debug" CACHE INTERNAL "Thumb debug C flags" FORCE)
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 --dwarf2 --debug" CACHE INTERNAL "Thumb generic C++ flags" FORCE)

SET(CMAKE_C_FLAGS_RELEASE "-O2" CACHE INTERNAL "Thumb debug C flags" FORCE)
SET(CMAKE_CXX_FLAGS_RELEASE "-O2" CACHE INTERNAL "Thumb generic C++ flags" FORCE)

SET(CMAKE_LIBRARY_PATH_FLAG "--userlibpath=")

# Set up the linker command line for linking executables and shared lib
SET(LINK_EXECUTABLE_FLAGS       "--diag_suppress=6314W,6304,6385,6632,1294-D --libpath=${RVCT_LIB}")
SET(CREATE_SHARED_LIBRARY_FLAGS                                             "--libpath=${RVCT_LIB}")

SET(CMAKE_C_LINK_EXECUTABLE
    "${CMAKE_LINKER} ${LINK_EXECUTABLE_FLAGS} <LINK_FLAGS> --output=<TARGET> <OBJECTS> <LINK_LIBRARIES>")

SET(CMAKE_CXX_LINK_EXECUTABLE
    "${CMAKE_LINKER} ${LINK_EXECUTABLE_FLAGS} <LINK_FLAGS> --output=<TARGET> <OBJECTS> <LINK_LIBRARIES>")

SET(CMAKE_C_CREATE_SHARED_LIBRARY
    "${CMAKE_LINKER} ${CREATE_SHARED_LIBRARY_FLAGS} <LINK_FLAGS> <LINK_LIBRARIES> --output=<TARGET> --symdefs=<TARGET_IMPLIB>${SYMDEFS_RAW_SUFFIX} <OBJECTS>"
    ${SYMDEFS_POST_BUILD})

SET(CMAKE_CXX_CREATE_SHARED_LIBRARY
    "${CMAKE_LINKER} ${CREATE_SHARED_LIBRARY_FLAGS} <LINK_FLAGS> <LINK_LIBRARIES> --output=<TARGET> --symdefs=<TARGET_IMPLIB>${SYMDEFS_RAW_SUFFIX} <OBJECTS>"
    ${SYMDEFS_POST_BUILD})

SET(CMAKE_ASM_RVCT_CREATE_SHARED_LIBRARY
    "${CMAKE_LINKER} ${CREATE_SHARED_LIBRARY_FLAGS} <LINK_FLAGS> <LINK_LIBRARIES> --output=<TARGET> --symdefs=<TARGET_IMPLIB>${SYMDEFS_RAW_SUFFIX} <OBJECTS>"
    ${SYMDEFS_POST_BUILD})

# TODO: Add more linker flags here - for example, debug/release?
SET(CMAKE_C_ARCHIVE_CREATE "${CMAKE_AR} --create -r <TARGET> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_APPEND "${CMAKE_AR} -r <TARGET> <OBJECTS>")
#SET(CMAKE_C_ARCHIVE_FINISH "true")
SET(CMAKE_CXX_ARCHIVE_CREATE ${CMAKE_C_ARCHIVE_CREATE})
SET(CMAKE_CXX_ARCHIVE_APPEND ${CMAKE_C_ARCHIVE_APPEND})
#SET(CMAKE_CXX_ARCHIVE_FINISH ${CMAKE_C_ARCHIVE_FINISH})


# Don't let CMake override the parameters we've set, e.g. CMAKE_LIBRARY_PATH_FLAG
SET(CMAKE_SYSTEM_SPECIFIC_INFORMATION_LOADED TRUE)

INCLUDE_DIRECTORIES(SYSTEM "${RVCT_INC}")