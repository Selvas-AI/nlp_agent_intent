cd ..
mkdir msvc
cd msvc
del CMakeCache.txt
rmdir /Q /S CMakeFiles
rmdir /Q /S external
cmake .. -DBUILD_PYTHON=ON -G"Visual Studio 14 Win64"
@pause