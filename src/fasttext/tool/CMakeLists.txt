get_source(SRCS ${CMAKE_CURRENT_SOURCE_DIR})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../src/cpp")

add_executable(fasttext_runner ${SRCS})
target_link_libraries(fasttext_runner ${FASTTEXT_LIBRARIES})
set_target_properties(fasttext_runner PROPERTIES FOLDER "Tools")
