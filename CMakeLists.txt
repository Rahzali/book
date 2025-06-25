cmake_minimum_required(VERSION 3.21)

project(MyQtApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.8.3 REQUIRED COMPONENTS Core Gui Widgets Sql)

add_executable(MyQtApp
        main.cpp
        mainwindow.ui
        database.cpp
        database.h
        mainwindow.cpp
        mainwindow.h
)

target_link_libraries(MyQtApp
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Sql
)
