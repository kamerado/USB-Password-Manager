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
find_package(QT NAMES Qt6 COMPONENTS Sql Widgets Qt6Concurrent REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Sql Widgets REQUIRED)
add_subdirectory(libs/spdlog)

# JsonCpp
option(JSONCPP_WITH_TESTS OFF)  # Disable tests
option(JSONCPP_WITH_POST_BUILD_UNITTEST OFF)  # Disable post-build tests
add_subdirectory(libs/jsoncpp)

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

# After your existing Qt and spdlog configuration

# ASIO (header-only)
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/asio/asio/include/asio.hpp")
    message(FATAL_ERROR "ASIO submodule not initialized. Please run: git submodule update --init --recursive")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/asio/asio/include
)

# Tell ASIO to be standalone (no Boost)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ASIO_STANDALONE
    ASIO_HAS_STD_CHRONO
)

# WebSocket++ (header-only)
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/websocketpp/websocketpp/client.hpp")
    message(FATAL_ERROR "WebSocket++ submodule not initialized. Please run: git submodule update --init --recursive")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/websocketpp
)
  target_link_libraries(nativegui PRIVATE
      Qt${QT_VERSION_MAJOR}::Widgets
      crypto++
      Qt${QT_VERSION_MAJOR}::Sql
      stdc++
      spdlog::spdlog
      jsoncpp_lib
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
