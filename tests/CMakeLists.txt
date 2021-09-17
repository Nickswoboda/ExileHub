cmake_minimum_required(VERSION 3.5)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network Gui Test WebEngineWidgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

enable_testing(true)

add_executable(auto_update_test auto_update_test.cpp ../src/auto_updater.cpp)
target_include_directories(auto_update_test PUBLIC ../src)
target_link_libraries(auto_update_test PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(auto_update_test PRIVATE Qt${QT_VERSION_MAJOR}::Network QuaZip::QuaZip)
add_test(NAME auto_update_test COMMAND auto_update_test)

add_executable(tray_icon_test tray_icon_test.cpp ../src/main_window.cpp ../src/tray_icon.cpp ../src/options_dialog.cpp ../src/browser.cpp ../src/navigation_area.cpp
    ../src/navigation_area.ui)
target_include_directories(tray_icon_test PUBLIC ../src)
target_link_libraries(tray_icon_test PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(tray_icon_test PRIVATE Qt${QT_VERSION_MAJOR}::Network QuaZip::QuaZip)
target_link_libraries(tray_icon_test PRIVATE Qt${QT_VERSION_MAJOR}::WebEngineWidgets)
add_test(NAME tray_icon_test COMMAND tray_icon_test)
