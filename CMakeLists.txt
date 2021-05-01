cmake_minimum_required(VERSION 3.5)

project(ExileHub LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/main_window.cpp
        src/main_window.hpp
        src/main_window.ui
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

target_link_libraries(ExileHub PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
