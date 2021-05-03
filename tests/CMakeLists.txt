cmake_minimum_required(VERSION 3.5)

find_package(Qt6 COMPONENTS Gui Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

enable_testing(true)

qt_add_executable(test_temp test_temp.cpp)
target_link_libraries(test_temp PRIVATE Qt${QT_VERSION_MAJOR}::Test)
add_test(NAME test_temp COMMAND test_temp)
