cmake_minimum_required(VERSION 3.5)

project(ExileHub LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Network REQUIRED)
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
        src/system_tray.cpp
        src/system_tray.hpp
        assets/assets.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ExileHub
        ${PROJECT_SOURCES}
    )
else()
    add_executable(ExileHub
        ${PROJECT_SOURCES}
    )

endif()

add_subdirectory(tests)

option(BUILD_SHARED_LIBS "" OFF)
set(QUAZIP_QT_MAJOR_VERSION 6 CACHE STRING "Overridden to 6")
add_subdirectory(dependencies/quazip)

target_link_libraries(ExileHub PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ExileHub PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(ExileHub PRIVATE QuaZip::QuaZip)

