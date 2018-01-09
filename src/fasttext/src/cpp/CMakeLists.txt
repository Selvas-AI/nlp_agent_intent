get_source(SRCS ${CMAKE_CURRENT_SOURCE_DIR})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_library(fasttext OBJECT ${SRCS})
set_target_properties(fasttext PROPERTIES FOLDER "Engine")