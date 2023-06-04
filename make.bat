mkdir build
cd build
cmake ..
cd ..
cmake --build build --config Release
move /Y build\Release\dsrs.exe dsrs.exe
dsrs