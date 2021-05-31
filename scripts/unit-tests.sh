cd $(dirname "$0")
cd ..
if [ ! -d "build/ExileHub-Desktop_Qt_5_15_2_MSVC2019_64bit-Debug" ]; then 
    mkdir -p build/ExileHub-Desktop_Qt_5_15_2_MSVC2019_64bit-Debug
fi
cd build/ExileHub-Desktop_Qt_5_15_2_MSVC2019_64bit-Debug

#Because WebEngineWidgets isn't supported by mingw we have to use MSVC
#Because we are using MSVC, this command needs to be run in either
#the x64 Native Tools Command Prompt for VS2019
#OR you must set up the environment variable in a different command prompt
#using the vcvars64.bat file

cmake -S C:/dev/ExileHub \
-GNinja "-DCMAKE_BUILD_TYPE:String=Debug" \
"-DQT_QMAKE_EXECUTABLE:STRING=C:/Qt/5.15.2/msvc2019_64/bin/qmake.exe" \
"-DCMAKE_PREFIX_PATH:STRING=C:/Qt/5.15.2/msvc2019_64" \
"-DCMAKE_C_COMPILER:STRING=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29910/bin/HostX64/x64/cl.exe" \
"-DCMAKE_CXX_COMPILER:STRING=C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29910/bin/HostX64/x64/cl.exe"
../..

Ninja

#Must have Qt .dlls in PATH
cd tests
ctest

exit $?