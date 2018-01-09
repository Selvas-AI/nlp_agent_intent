macro(get_platform_name PLATFORM TARGET_ABI)
  set(${TARGET_ABI} "x86")
  if(APPLE)
    if(NOT OPTION_FORCE_BUILD_32BIT)
      set(BITNESS 64)
      set(${TARGET_ABI} "x86_64")
    else()
      set(BITNESS 32)
      set(${TARGET_ABI} "i386")
    endif()
  else()
    if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND NOT OPTION_FORCE_BUILD_32BIT)
      set(BITNESS 64)
      set(${TARGET_ABI} "x86_64")
    else()
      set(BITNESS 32)
    endif()
  endif()
  if(MSVC)
    set(${PLATFORM} "win32") 
  elseif(RVCT)
    set(${PLATFORM} "rvct") 
    set(${TARGET_ABI} "arm")
  elseif(ANDROID)
    set(${PLATFORM} "android") 
    set(${TARGET_ABI} ${ANDROID_ABI})
  elseif(TIZEN)
    set(${PLATFORM} "arm")
    #set(${PLATFORM} "i386")
  elseif(APPLE)
    if(NOT DEFINED IOS_PLATFORM)
      set(${PLATFORM} "darwin") 
    else()
      set(${PLATFORM} "darwin_ios") 
      if(NOT DEFINED IOS_ARCH)
        set(${TARGET_ABI} "armv7")
      else()
        set(${TARGET_ABI} ${IOS_ARCH})
      endif()
    endif()
  elseif(WIN32)
    set(${PLATFORM} "mingw") 
  elseif(UNIX)
    set(${PLATFORM} "linux") 
  else()
    message(SEND_ERROR "Unknown TARGET_ABI")
  endif()
endmacro()

macro(get_base_name_list BASE_LIST)
  set(RESULT)
  foreach(FILE_PATH ${ARGV})
    get_filename_component(FILENAME ${FILE_PATH} NAME)
    list(APPEND RESULT ${FILENAME}) 
  endforeach()
  set(${BASE_LIST} ${RESULT})
endmacro(get_base_name_list)

function(print_items)
  foreach(ITEM ${ARGV})
    message(STATUS "\t" ${ITEM})
  endforeach()
endfunction(print_items)

macro(get_revision SVN_DIR RESULT)
message(STATUS "SVN_DIR: ${SVN_DIR}")
find_package(Subversion)
if(Subversion_FOUND)
  Subversion_WC_INFO(${SVN_DIR} ${RESULT})
else()
  message(SEND_ERROR "SVN package not exist")
endif(Subversion_FOUND)
endmacro(get_revision)

macro(make_window_path src dst)
  STRING(REPLACE "/" "\\" ${dst} "${src}")
  SET(${dst} ${${dst}})
endmacro(make_window_path)

# remove items of a pattern from a list
# usage: filter_out_list("${SRCS}" "dnntrain/cuda" SRCS)
macro(filter_out_list SOURCE FILTER TARGET)
    foreach (_ITEM ${SOURCE})
        if (${_ITEM} MATCHES ${FILTER})
            list(APPEND OUT_ITEMS ${_ITEM})
        endif()
    endforeach()
    set(${TARGET} ${SOURCE})
    list(REMOVE_ITEM ${TARGET} ${OUT_ITEMS})
endmacro()

macro(getfilelist srcs curdir begin)
  if(${ARGC} GREATER 3)
    set(ex_list "")
    foreach(arg ${ARGN})
      if("${arg}" STREQUAL "INCLUDE")
        set(ex_command ${arg})
      elseif("${arg}" STREQUAL "EXCLUDE")
        set(ex_command ${arg})
      else()
        list(APPEND ex_list ${arg})
      endif()
    endforeach()
    if("${ex_command}" STREQUAL "INCLUDE")    
      list(APPEND ex_list include)
      list(APPEND ex_list src)    
    endif()
  endif()
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
        if(ex_command STREQUAL "INCLUDE")
          set(allow_child "THIS IS EMPTY")
          foreach(ex_name ${ex_list})
            if(${ex_name} STREQUAL ${child})
              set(allow_child ${child})
              break()
            endif()
          endforeach() 
          set(child ${allow_child})
        elseif(ex_command STREQUAL "EXCLUDE")
          foreach(ex_name ${ex_list})
            if(ex_name STREQUAL child)
              set(child "THIS IS EMPTY")
              break()
            endif()
          endforeach()
        endif()
        if(NOT child STREQUAL "THIS IS EMPTY")
          if("${begin}" STREQUAL "")
            if(child STREQUAL "include" OR child STREQUAL "src" OR child STREQUAL "source")
              getfilelist(${srcs} "${curdir}/${child}" "")
            else()
              getfilelist(${srcs} "${curdir}/${child}" "${child}")
            endif()
          else()
            getfilelist(${srcs} "${curdir}/${child}" "${begin}/${child}")
          endif()
        endif()
      else()
        get_filename_component(extension "${child}" EXT)
        if(extension STREQUAL ".c" OR extension STREQUAL ".cc"
            OR extension STREQUAL ".cpp" OR extension STREQUAL ".h"
            OR extension STREQUAL ".cu" OR extension STREQUAL ".cuh")
          set(${srcs} "${${srcs}};${curdir}/${child}")
          if(NOT "${begin}" STREQUAL "")
            string(REPLACE "/" "\\" winstyle_begin "${begin}")
            source_group("${winstyle_begin}" FILES ${curdir}/${child})
          endif()
        endif()
    endif()
  endforeach()
endmacro()

macro(get_current_date CURRENT_DATE)
  if(CMAKE_HOST_SYSTEM MATCHES Window)
    execute_process(COMMAND "${BUILD_POLICY_DIR}/programs/get_date.exe" OUTPUT_VARIABLE RESULT)
  else()
    execute_process(COMMAND "date" "+%y/%m/%d" OUTPUT_VARIABLE RESULT)
  endif()
  string(REGEX REPLACE "/" "" RESULT ${RESULT})
  string(REGEX REPLACE "\n" "" RESULT ${RESULT})
  set(${CURRENT_DATE} ${RESULT})
endmacro(get_current_date)

macro(get_source srcs dir)
  if(${ARGC} GREATER 3)
    getfilelist(${srcs} ${dir} "" ${ARGN})
  else()
    getfilelist(${srcs} ${dir} "")
  endif()
endmacro()

MACRO(GIT_WC_INFO dir prefix)
  EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} rev-list -n 1 HEAD
     WORKING_DIRECTORY ${dir}
     ERROR_VARIABLE GIT_error
     OUTPUT_VARIABLE ${prefix}_WC_REVISION_HASH
     OUTPUT_STRIP_TRAILING_WHITESPACE)
  SET(${prefix}_WC_REVISION ${${prefix}_WC_REVISION_HASH})
  if(NOT ${GIT_error} EQUAL 0)
    message(SEND_ERROR "Command \"${GIT_EXECUTBALE} rev-list -n 1 HEAD\" in directory ${dir} failed with output:\n${GIT_error}")
  ELSE(NOT ${GIT_error} EQUAL 0)
    EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} name-rev ${${prefix}_WC_REVISION_HASH}
       WORKING_DIRECTORY ${dir}
       OUTPUT_VARIABLE ${prefix}_WC_REVISION_NAME
        OUTPUT_STRIP_TRAILING_WHITESPACE)
  ENDIF(NOT ${GIT_error} EQUAL 0)
  
  EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} config --get remote.origin.url
     WORKING_DIRECTORY ${dir}
     OUTPUT_VARIABLE ${prefix}_WC_URL
     OUTPUT_STRIP_TRAILING_WHITESPACE)
  
  SET(${prefix}_WC_ROOT ${${prefix}_WC_URL})
  SET(${prefix}_WC_GITSVN False)
  
  # In case git-svn is used, attempt to extract svn info
  EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} svn info
    WORKING_DIRECTORY ${dir}
    TIMEOUT 3
    ERROR_VARIABLE git_svn_info_error
    OUTPUT_VARIABLE ${prefix}_WC_INFO
    RESULT_VARIABLE git_svn_info_result
    OUTPUT_STRIP_TRAILING_WHITESPACE)
    
  IF(${git_svn_info_result} EQUAL 0)
    SET(${prefix}_WC_GITSVN True)
    STRING(REGEX REPLACE "^(.*\n)?URL: ([^\n]+).*"
      "\\2" ${prefix}_WC_URL "${${prefix}_WC_INFO}")
    STRING(REGEX REPLACE "^(.*\n)?Revision: ([^\n]+).*"
      "\\2" ${prefix}_WC_REVISION "${${prefix}_WC_INFO}")
    STRING(REGEX REPLACE "^(.*\n)?Repository Root: ([^\n]+).*"
      "\\2" ${prefix}_WC_ROOT "${${prefix}_WC_INFO}")
    STRING(REGEX REPLACE "^(.*\n)?Last Changed Author: ([^\n]+).*"
      "\\2" ${prefix}_WC_LAST_CHANGED_AUTHOR "${${prefix}_WC_INFO}")
    STRING(REGEX REPLACE "^(.*\n)?Last Changed Rev: ([^\n]+).*"
      "\\2" ${prefix}_WC_LAST_CHANGED_REV "${${prefix}_WC_INFO}")
    STRING(REGEX REPLACE "^(.*\n)?Last Changed Date: ([^\n]+).*"
      "\\2" ${prefix}_WC_LAST_CHANGED_DATE "${${prefix}_WC_INFO}")
  ENDIF(${git_svn_info_result} EQUAL 0)
  
ENDMACRO(GIT_WC_INFO)

function(join VALUES GLUE OUTPUT)
  string (REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
  string (REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
  set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()

macro(disable_compile_warning)
  if(MSVC)
    STRING(REGEX REPLACE "/W." "/W0" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
    STRING(REGEX REPLACE "/W." "/W0" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
  endif()
endmacro()

function(make_archive src dst)
  if(EXISTS ${dst})
    file(GLOB_RECURSE ARCHIVE_TARGET_FILES ${src}/*)
    set(RESOURCE_UPDATE_NEED 0)
    foreach(ELEMENT ${ARCHIVE_TARGET_FILES})
      if(${ELEMENT} IS_NEWER_THAN ${dst})
        set(RESOURCE_UPDATE_NEED 1)
        break()
      endif()
    endforeach()
    if (${RESOURCE_UPDATE_NEED})
      file(REMOVE ${dst})
    endif()
  endif()
  if(NOT EXISTS ${dst})
    get_filename_component(dst_path ${dst} PATH)
    if(NOT EXISTS ${dst_path})
      file(MAKE_DIRECTORY ${dst_path})
    endif()
    if(EXISTS ${src})
      execute_process(
        COMMAND
        "${PYTHON_EXECUTABLE}" "${BUILD_POLICY_DIR}/programs/compression.py" "${src}" "${dst}"
        RESULT_VARIABLE _result
      )
    endif()
  endif()
endfunction(make_archive)

# !todo(kyoungjoo) �ӽ� �ڵ�(find_alternative_library_path �Լ��� ���յǾ�� ��)
function(find_alternative_library_path_on_windows BASE_PATH TARGET_PLATFORM_NAME LIBRARY_PATH) 
  if(MSVC)
    if(MSVC14)
      set(MSVC_VER_FLAG "v140")
    elseif(MSVC12)
      set(MSVC_VER_FLAG "v120")
    elseif(MSVC11)
      set(MSVC_VER_FLAG "v110")
    elseif(MSVC10)
      set(MSVC_VER_FLAG "v100")
    elseif(MSVC9)  
      set(MSVC_VER_FLAG "v90")
    endif()
  endif()
  set(SELECTED_PLATFORM_NAME ${TARGET_PLATFORM_NAME})
  if(${SELECTED_PLATFORM_NAME} MATCHES ".+x86_64$" AND OPTION_FORCE_BUILD_32BIT)
    string(REGEX REPLACE "_64$" "" SELECTED_PLATFORM_NAME ${SELECTED_PLATFORM_NAME}) 
  endif()
  set(LIB_PATH "${BASE_PATH}/libs/${SELECTED_PLATFORM_NAME}/${MSVC_VER_FLAG}")
  if(NOT EXISTS ${LIB_PATH})
      if(${SELECTED_PLATFORM_NAME} MATCHES "^android_armeabi.*")
        set(SELECTED_PLATFORM_NAME "android_armeabi")
      else()
        set(SELECTED_PLATFORM_NAME ${SELECTED_PLATFORM_NAME})
      endif()
      set(LIB_PATH "${BASE_PATH}/libs/${SELECTED_PLATFORM_NAME}/${MSVC_VER_FLAG}")
  endif()
  set(${LIBRARY_PATH} ${LIB_PATH} PARENT_SCOPE)
endfunction(find_alternative_library_path_on_windows)

function(find_alternative_library_path BASE_PATH TARGET_PLATFORM_NAME LIBRARY_PATH)
  set(SELECTED_PLATFORM_NAME ${TARGET_PLATFORM_NAME})
  if(${SELECTED_PLATFORM_NAME} MATCHES ".+x86_64$" AND OPTION_FORCE_BUILD_32BIT)
    string(REGEX REPLACE "_64$" "" SELECTED_PLATFORM_NAME ${SELECTED_PLATFORM_NAME}) 
  endif()
  set(LIB_PATH "${BASE_PATH}/libs/${SELECTED_PLATFORM_NAME}")
  if(NOT EXISTS ${LIB_PATH})
      if(${SELECTED_PLATFORM_NAME} MATCHES "^android_armeabi.*")
        set(SELECTED_PLATFORM_NAME "android_armeabi")
      else()
        set(SELECTED_PLATFORM_NAME ${SELECTED_PLATFORM_NAME})
      endif()
      set(LIB_PATH "${BASE_PATH}/libs/${SELECTED_PLATFORM_NAME}")
  endif()
  set(${LIBRARY_PATH} ${LIB_PATH} PARENT_SCOPE)
endfunction()

function(find_alternative_libraries_with_type BASE_PATH TARGET_PLATFORM_NAME LIBRARY_TYPE LIBRARIES)
  find_alternative_library_path(${BASE_PATH} ${TARGET_PLATFORM_NAME} LIBRARY_PATH)
  if(MSVC)
    if(MSVC14)
      set(MSVC_VER_FLAG "v140")
    elseif(MSVC12)
      set(MSVC_VER_FLAG "v120")
    elseif(MSVC11)
      set(MSVC_VER_FLAG "v110")
    elseif(MSVC10)
      set(MSVC_VER_FLAG "v100")
    elseif(MSVC9)  
      set(MSVC_VER_FLAG "v90")
    endif()
    set(SPECIFIC_LIBRARY_PATH "${LIBRARY_PATH}/Release/${MSVC_VER_FLAG}")
    file(GLOB SPECIFIC_FILES "${SPECIFIC_LIBRARY_PATH}/*${LIBRARY_TYPE}")
    foreach(FILE_PATH ${SPECIFIC_FILES})
      get_filename_component(FILENAME ${FILE_PATH} NAME)
      set(CURFILE debug "${LIBRARY_PATH}/Debug/${MSVC_VER_FLAG}/${FILENAME}"
                  optimized "${LIBRARY_PATH}/Release/${MSVC_VER_FLAG}/${FILENAME}")
      set(LIBS ${LIBS} ${CURFILE})
    endforeach()
  endif()
  if(NOT LIBS)
    file(GLOB SPECIFIC_FILES "${LIBRARY_PATH}/*${LIBRARY_TYPE}")
    foreach(FILE_PATH ${SPECIFIC_FILES})
      get_filename_component(FILENAME ${FILE_PATH} NAME)
      set(CURFILE "${LIBRARY_PATH}//${FILENAME}")
      set(LIBS ${LIBS} ${CURFILE})
    endforeach()
  endif()
  set(${LIBRARIES} ${LIBS} PARENT_SCOPE)
endfunction()

function(find_alternative_libraries BASE_PATH TARGET_PLATFORM_NAME LIBRARIES FILE_LIST)
 find_alternative_libraries_with_type(${BASE_PATH} ${TARGET_PLATFORM_NAME} ${CMAKE_STATIC_LIBRARY_SUFFIX} LIBS_STATIC)
 find_alternative_libraries_with_type(${BASE_PATH} ${TARGET_PLATFORM_NAME} ${CMAKE_SHARED_LIBRARY_SUFFIX} LIBS_SHARED)
 set(LIBS ${LIBS} ${LIBS_STATIC})
 if(MSVC)
   set(LIBS ${LIBS} ${LIBS_SHARED})
 else()
  foreach(LIB_SHARED_ELEMENT ${LIBS_SHARED})
    get_filename_component(FILENAME ${LIB_SHARED_ELEMENT} NAME)
    STRING(REGEX REPLACE "lib" "" CONVERTED ${FILENAME})
    set(LIBS ${LIBS} ${CONVERTED})
  endforeach()
 endif()
 set(${FILE_LIST} ${LIBS_STATIC} ${LIBS_SHARED} PARENT_SCOPE)
 set(${LIBRARIES} ${LIBS} PARENT_SCOPE)
endfunction()

function(install_local_library LIBRARY TARGET_PATH)
  if("${ARGN}" STREQUAL "")
    install(FILES ${LIBRARY} DESTINATION ${TARGET_PATH})
  else()
    get_filename_component(FILEEXT ${LIBRARY} EXT)
    if("${ARGN}" STREQUAL "static")
      if("${FILEEXT}" STREQUAL "${CMAKE_STATIC_LIBRARY_SUFFIX}")
        install(FILES ${LIBRARY} DESTINATION ${TARGET_PATH})
      endif()
    elseif("${ARGN}" STREQUAL "shared")
      if("${FILEEXT}" STREQUAL "${CMAKE_SHARED_LIBRARY_SUFFIX}")
        install(FILES ${LIBRARY} DESTINATION ${TARGET_PATH})
      endif()
    else()
      message(WARNING "Function parameter invalid")
    endif()
  endif()
endfunction()

function(install_local_library_msvc LIBRARY CONFIGURATION_TYPE TARGET_PATH)
  if("${ARGN}" STREQUAL "")
    install(FILES ${LIBRARY} DESTINATION ${TARGET_PATH}
            CONFIGURATIONS ${CONFIGURATION_TYPE})
  else()
    get_filename_component(FILEEXT ${LIBRARY} EXT)
    if("${ARGN}" STREQUAL "static")
      if("${FILEEXT}" STREQUAL "${CMAKE_STATIC_LIBRARY_SUFFIX}")
        install(FILES ${LIBRARY} DESTINATION ${TARGET_PATH}
                CONFIGURATIONS ${CONFIGURATION_TYPE})
      endif()
    elseif("${ARGN}" STREQUAL "shared")
      if("${FILEEXT}" STREQUAL "${CMAKE_SHARED_LIBRARY_SUFFIX}")
        install(FILES ${LIBRARY} DESTINATION ${TARGET_PATH}
                CONFIGURATIONS ${CONFIGURATION_TYPE})
      endif()
    else()
      message(WARNING "Function parameter invalid")
    endif()
  endif()
endfunction()

function(install_local_libraries LIBRARIES TARGET_PATH)
  string(COMPARE GREATER "${${LIBRARIES}}" "debug" IS_MSVC_TYPE)
  if(IS_MSVC_TYPE)
    set(TYPE_CHECK_SWITCH ON)
    set(INSTALL_SWITCH OFF)
    foreach(TOKEN ${${LIBRARIES}})
      if (TYPE_CHECK_SWITCH)
        if(TOKEN STREQUAL "debug")
          set(INSTALL_SWITCH ON)
          set(CONFIGURATION_TYPE "debug")
        elseif(TOKEN STREQUAL "optimized")
          set(INSTALL_SWITCH ON)
          set(CONFIGURATION_TYPE "release")
        endif()
        set(TYPE_CHECK_SWITCH OFF)
      else()
        if(INSTALL_SWITCH)
          install_local_library_msvc(${TOKEN} ${CONFIGURATION_TYPE} ${TARGET_PATH} ${ARGN})
          set(INSTALL_SWITCH OFF)
        endif()
        set(TYPE_CHECK_SWITCH ON)
      endif()
    endforeach()
  else()
    foreach(TOKEN ${${LIBRARIES}})
      install_local_library(${TOKEN} ${TARGET_PATH} ${ARGN})
    endforeach()
  endif()
endfunction()

macro(get_file_content path content)
  if(CMAKE_HOST_SYSTEM MATCHES Window)
    set(cat_prog "${BUILD_POLICY_DIR}\\programs\\cat.exe")
    make_window_path(${path} args)
    message(STATUS ${args})
  else()
    set(cat_prog cat)
    set(args ${path})
  endif()
  exec_program(${cat_prog} ARGS ${args} OUTPUT_VARIABLE ${content})
endmacro()

macro(set_mt_flags)
  set(CMAKE_USER_MAKE_RULES_OVERRIDE
    ${BUILD_POLICY_DIR}/c_flag_overrides.cmake)
  set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX
    ${BUILD_POLICY_DIR}/cxx_flag_overrides.cmake)
endmacro()