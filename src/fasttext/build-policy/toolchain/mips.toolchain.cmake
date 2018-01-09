set ( CMAKE_SYSTEM_NAME Linux )
set ( CMAKE_SYSTEM_VERSION 1 )

# Cross compiler
set ( CMAKE_C_COMPILER mips-linux-gnu-gcc )
set ( CMAKE_CXX_COMPILER mips-linux-gnu-g++ )
set ( CMAKE_FIND_ROOT_PATH /usr/mips-linux-gcc/lib/ )
set ( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
set ( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
set ( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )

# Additional configuration follows
add_definitions ( -fno-strict-aliasing )
