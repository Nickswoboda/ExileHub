cd $(dirname "$0")
cd ..
if [ -d "build" ]; then 
    rm -r build
fi
mkdir build
cd build

#QtWebEngine only supports MSVC
#You must either run the script in the x64 Native Tools Command Prompt for VS2019
#OR
#Run the vcvars64.bat file before this script
#OR
#Set the CMAKE C and CXX compiler options to MSCV/cl

cmake .. -G Ninja

Ninja

#Must have Qt .dlls in PATH
cd tests
ctest

exit $?