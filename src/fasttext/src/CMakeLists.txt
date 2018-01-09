#####################################################################
# Build engines
add_subdirectory(cpp)
if(BUILD_PYTHON)
  add_subdirectory(python)
  add_subdirectory(jsoncpp)
endif()

set(FASTTEXT_OBJS $<TARGET_OBJECTS:fasttext>)

add_library(libfasttext STATIC ${FASTTEXT_OBJS})

set_target_properties(libfasttext PROPERTIES FOLDER "FASTTEXT-SDK")

set(FASTTEXT_LIBRARIES libfasttext)
set(FASTTEXT_LIBRARIES ${FASTTEXT_LIBRARIES} PARENT_SCOPE)
