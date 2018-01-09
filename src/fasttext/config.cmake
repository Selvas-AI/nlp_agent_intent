########  #############################################################
# Setting of the engine version
#
# Note: If you release the engine of the major or minor and patch version
#       You Should be change the version value manualy   
set(FASTTEXT_MAJOR_VERSION 1 CACHE INTEGER "Major Version")
set(FASTTEXT_MINOR_VERSION 0 CACHE INTEGER "Minor Version")
set(FASTTEXT_PATCH_VERSION 0 CACHE INTEGER "Patch Version")

# Generate a library version
if(NOT DEFINED FASTTEXT_BUILD_VERSION)
    # Unstable version: 1.0.0
    set(FASTTEXT_BUILD_VERSION )
else()
    # Stable version: 1.0.0
    set(FASTTEXT_BUILD_VERSION ${GIT_HASH_VALUE})
endif()
set(FASTTEXT_VERSION
  FASTTEXT.${FASTTEXT_MAJOR_VERSION}.${FASTTEXT_MINOR_VERSION}.${FASTTEXT_PATCH_VERSION}.${GIT_HASH_VALUE}\(${BUILD_DATE}\))
