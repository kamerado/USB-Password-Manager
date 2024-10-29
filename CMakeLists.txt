cmake_minimum_required(VERSION 3.30.1)

project(nativegui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER gcc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Sql Widgets Qt6Concurrent  REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Sql Widgets REQUIRED)

link_directories("${PROJECT_SOURCE_DIR}/libs")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_DIR}/")

set(PROJECT_SOURCES
        src/main.cpp
        src/gui/GUI/mainwindow.cpp
        src/gui/GUI/mainwindow.h
        src/gui/GUI/mainwindow.ui
        src/gui/GUI/login.cpp
        src/gui/GUI/login.h
        src/gui/GUI/login.ui
        src/core/DatabaseManager.h
        src/core/DatabaseManager.cpp
        src/core/EncryptionUtil.h
        src/core/EncryptionUtil.cpp
        src/core/Logger.cpp
        src/core/Logger.h
        src/core/WebSocket.cpp
        src/core/WebSocket.h

        src/gui/GUI/setup.ui
        src/gui/GUI/setup.h
        src/gui/GUI/setup.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(nativegui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
  )
  target_link_libraries(nativegui PRIVATE
      Qt${QT_VERSION_MAJOR}::Widgets
      crypto++
      Qt${QT_VERSION_MAJOR}::Sql
      stdc++
  )

  set_target_properties(nativegui PROPERTIES
      MACOSX_BUNDLE_GUI_IDENTIFIER com.kamerado.native_app
      MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
      MACOSX_BUNDLE_SHORT_VERSION_STRING
          ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
      MACOSX_BUNDLE TRUE
      WIN32_EXECUTABLE TRUE
      RUNTIME_OUTPUT_DIRECTORY
          /home/meganuts/projects/USB-Password-Manager/build/
  )

endif()
if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(nativegui)
endif()
