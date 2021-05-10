cd $(dirname "$0")
cd ..
if [ ! -d "build/ExileHub-Desktop_Qt_6_1_0_MinGW_64_bit-Debug" ]; then 
    mkdir -p build/ExileHub-Desktop_Qt_6_1_0_MinGW_64_bit-Debug
fi
cd build/ExileHub-Desktop_Qt_6_1_0_MinGW_64_bit-Debug

cmake -DCMAKE_BUILD_TYPE:STRING="Debug" \
-DCMAKE_CXX_COMPILER:STRING="C:/Qt/Tools/mingw810_64/bin/g++.exe" \
-DCMAKE_C_COMPILER:STRING="C:/Qt/Tools/mingw810_64/bin/gcc.exe" \
-DCMAKE_PREFIX_PATH:STRING="C:/Qt/6.1.0/mingw81_64" \
-DQT_QMAKE_EXECUTABLE:STRING="C:/Qt/6.1.0/mingw81_64/bin/qmake.exe" \
-G"Ninja" \
../..

Ninja

#Must have Qt .dlls in PATH
cd tests
ctest

exit $?