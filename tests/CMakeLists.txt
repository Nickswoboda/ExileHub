cmake_minimum_required(VERSION 3.5)

find_package(Qt6 COMPONENTS Gui Test REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

enable_testing(true)

qt_add_executable(test_temp test_temp.cpp ../src/main_window.cpp)
target_include_directories(test_temp PUBLIC ../src)
target_link_libraries(test_temp PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Widgets)
add_test(NAME test_temp COMMAND test_temp)
