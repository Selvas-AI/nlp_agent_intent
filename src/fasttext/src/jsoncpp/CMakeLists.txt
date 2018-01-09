get_source(SRCS ${CMAKE_CURRENT_SOURCE_DIR})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_library(jsoncpp OBJECT ${SRCS})
set_target_properties(jsoncpp PROPERTIES FOLDER "Engine")