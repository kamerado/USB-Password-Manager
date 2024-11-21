cmake_minimum_required(VERSION 3.30.1)

project(nativegui VERSION 0.1 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# # Set CMake policies
# cmake_policy(SET CMP0177 NEW)

# set(CMAKE_C_COMPILER gcc)
# set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/libs)

# # Set Qt paths for Windows
# set(CMAKE_PREFIX_PATH "C:/Qt/6.8.0/mingw_64")
# set(Qt6_DIR "C:/Qt/6.8.0/mingw_64/lib/cmake/Qt6")

# # Add Qt submodule
# add_subdirectory(${EXTERNAL_DIR}/qt5)

add_subdirectory(${EXTERNAL_DIR}/spdlog)
include_directories(${EXTERNAL_DIR}/spdlog/include)

include_directories(${EXTERNAL_DIR}/json/include)
include_directories("$ENV{HOME}/Qt/6.8.0/gcc_64/include/")

add_subdirectory(${EXTERNAL_DIR}/cryptopp-cmake)

find_package(QT NAMES Qt6 COMPONENTS
  Core Sql Widgets WebSockets REQUIRED
)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
  Core Sql Widgets WebSockets REQUIRED
)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


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

qt_add_executable(nativegui
      MANUAL_FINALIZATION
      ${PROJECT_SOURCES}
)
target_link_libraries(nativegui PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    cryptopp::cryptopp
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::WebSockets
    stdc++
    spdlog::spdlog
)

set_target_properties(nativegui PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.kamerado.native_app
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING
        ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_SOURCE_DIR}/build/
)
if(PROJECT_IS_TOP_LEVEL AND UNIX)
  # Create symlink to compile_commands.json for IDE to pick it up
  execute_process(
      COMMAND ${CMAKE_COMMAND} -E create_symlink
          ${CMAKE_BINARY_DIR}/compile_commands.json
          ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )


endif()
if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(nativegui)
      # Copy required DLLs to output directory
      add_custom_command(TARGET nativegui POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:spdlog::spdlog>
          $<TARGET_FILE_DIR:nativegui>
  )
endif()
