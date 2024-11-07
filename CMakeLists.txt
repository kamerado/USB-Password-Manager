cmake_minimum_required(VERSION 3.30.1)

project(nativegui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/libs)

add_subdirectory(${EXTERNAL_DIR}/spdlog)
include_directories(${EXTERNAL_DIR}/spdlog/include)

include_directories(${EXTERNAL_DIR}/json/include)

include_directories(${EXTERNAL_DIR}/websocketpp)

add_subdirectory(${EXTERNAL_DIR}/boost)
include_directories(${EXTERNAL_DIR}/boost)

add_subdirectory(libs/cryptopp-cmake)

find_package(QT NAMES Qt6 COMPONENTS Sql Widgets Qt6Concurrent REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Sql Widgets REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp

        src/gui/GUI/mainwindow.cpp src/gui/GUI/mainwindow.h
        src/gui/GUI/mainwindow.ui

        src/gui/GUI/login.cpp src/gui/GUI/login.h src/gui/GUI/login.ui
        src/core/DatabaseManager.h src/core/DatabaseManager.cpp
        src/core/EncryptionUtil.h src/core/EncryptionUtil.cpp
        src/core/Logger.cpp src/core/Logger.h
        src/core/WebSocket.h src/core/WebSocket.cpp
        src/gui/GUI/setup.ui src/gui/GUI/setup.h src/gui/GUI/setup.cpp
        src/core/settings.h src/core/settings.cpp

        src/gui/GUI/settingsdialog.h src/gui/GUI/settingsdialog.cpp
        src/gui/GUI/settingsdialog.ui

        src/gui/GUI/inputdialog.h src/gui/GUI/inputdialog.cpp
        src/gui/GUI/inputdialog.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(nativegui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
  )
  target_link_libraries(nativegui PRIVATE
      Qt${QT_VERSION_MAJOR}::Widgets
      cryptopp
      Qt${QT_VERSION_MAJOR}::Sql
      stdc++
      spdlog::spdlog
      ${Boost_LIBRARIES}
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
