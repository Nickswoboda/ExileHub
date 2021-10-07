cmake_minimum_required(VERSION 3.5)

project(ExileHub LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Version 5 must be used until Qt6 supports QtWebEngine
set(QT_VERSION_MAJOR 5)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/application.cpp
        src/application.hpp
        src/main_window.cpp
        src/main_window.hpp
        src/main_window.ui
        src/auto_updater.cpp
        src/auto_updater.hpp
        src/tray_icon.cpp
        src/tray_icon.hpp
        src/options_dialog.cpp
        src/options_dialog.hpp
        src/options_dialog.ui
        src/add_app_dialog.cpp
        src/add_app_dialog.hpp
        src/add_app_dialog.ui
        src/app_manager.cpp
        src/app_manager.hpp
        src/app.cpp
        src/app.hpp
        src/apps_view.hpp
        src/apps_view.cpp
        src/apps_view.ui
        src/api_handler.cpp
        src/api_handler.hpp
        assets/assets.qrc

)

add_executable(ExileHub
    ${PROJECT_SOURCES}
)

add_subdirectory(tests)

option(BUILD_SHARED_LIBS "" OFF)
add_subdirectory(dependencies/quazip)

target_link_libraries(ExileHub PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ExileHub PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(ExileHub PRIVATE QuaZip::QuaZip)

