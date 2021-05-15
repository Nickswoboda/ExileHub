cmake_minimum_required(VERSION 3.5)

find_package(Qt6 COMPONENTS Gui Test REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets NetworkREQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

enable_testing(true)

qt_add_executable(auto_update_test test_auto_update.cpp ../src/auto_updater.cpp)
target_include_directories(auto_update_test PUBLIC ../src)
target_link_libraries(auto_update_test PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(auto_update_test PRIVATE Qt${QT_VERSION_MAJOR}::Network QuaZip::QuaZip)
add_test(NAME auto_update_test COMMAND auto_update_test)
