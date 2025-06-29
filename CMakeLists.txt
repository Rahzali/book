cmake_minimum_required(VERSION 3.21)

project(MyQtApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.8.3 REQUIRED COMPONENTS Core Gui Widgets Sql)

qt_standard_project_setup()

qt_add_executable(MyQtApp
        main.cpp
        mainwindow.ui
        database.cpp
        database.h
        mainwindow.cpp
        mainwindow.h
        rent.cpp
        rent.h
)

target_link_libraries(MyQtApp
        PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Sql
)

if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE_GUI_IDENTIFIER com.zofikar.mushromapp
            MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
            MACOSX_BUNDLE TRUE
    )
elseif (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "$<TARGET_FILE_DIR:Qt6::Core>/windeployqt6.exe"
        "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMENT "Running windeployqt6 after build")