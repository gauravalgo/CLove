mkdir BuildVC

cd BuildVC
cmake .. -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=../bin
pause