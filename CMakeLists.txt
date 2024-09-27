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

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

set(CEF_ROOT "${PROJECT_SOURCE_DIR}/libs/cef_binary_128.4.12+g1d7a1f9+chromium-128.0.6613.138_linux64")
set(CEF_DIR "${CEF_ROOT}/cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_DIR}/")

find_package(CEF REQUIRED)

include(${CEF_DIR}FindCEF.cmake)

include(${CEF_LIBRARIES_DIR}/cmake/cef_macros.cmake)
include(${CEF_LIBRARIES_DIR}/cmake/cef_variables.cmake)

include_directories(
  ${CEF_ROOT}/include
)


message("${PROJECT_SOURCE_DIR}")

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
        src/core/simpleHandler.cpp
        src/core/simpleHandler.h
        src/core/simpleApp.cpp
        src/core/simpleApp.h
        src/core/CefThread.cpp
        src/core/CefThread.h
        src/gui/GUI/setup.ui
        src/gui/GUI/setup.h
        src/gui/GUI/setup.cpp
)

set(CEF_INC "${PROJECT_SOURCE_DIR}/libs/cef/")

set(CEF_TEST_SOURCES
  tests/core_tests/CefThreadTests/cefclient_gtk.cc

  ${CEF_INC}tests/cefclient/browser/main_context_impl.h
  ${CEF_INC}tests/cefclient/browser/main_context_impl.cc
  ${CEF_INC}tests/cefclient/browser/main_message_loop_multithreaded_gtk.h
  ${CEF_INC}tests/cefclient/browser/main_message_loop_multithreaded_gtk.cc
  ${CEF_INC}include/base/cef_logging.h
  ${CEF_INC}include/cef_app.h
  ${CEF_INC}include/cef_command_line.h
  ${CEF_INC}include/wrapper/cef_helpers.h
  ${CEF_INC}tests/cefclient/browser/test_runner.h
  ${CEF_INC}tests/cefclient/browser/test_runner.cc
  ${CEF_INC}tests/shared/browser/client_app_browser.h
  ${CEF_INC}tests/shared/browser/client_app_browser.cc
  ${CEF_INC}tests/shared/browser/main_message_loop_external_pump.h
  ${CEF_INC}tests/shared/browser/main_message_loop_external_pump.cc
  ${CEF_INC}tests/shared/browser/main_message_loop_std.h
  ${CEF_INC}tests/shared/browser/main_message_loop_std.cc
  ${CEF_INC}tests/shared/common/client_app_other.h
  ${CEF_INC}tests/shared/common/client_app_other.cc
  ${CEF_INC}tests/shared/common/client_switches.h
  ${CEF_INC}tests/shared/common/client_switches.cc
  ${CEF_INC}tests/shared/renderer/client_app_renderer.h
  ${CEF_INC}tests/shared/renderer/client_app_renderer.cc

  src/core/simpleApp.cpp
  src/core/simpleApp.h
  src/core/simpleHandler.cpp
  src/core/simpleHandler.h
  src/core/CefThread.cpp
  src/core/CefThread.h

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
      ${PROJECT_SOURCE_DIR}/libs/libcef.so
      ${PROJECT_SOURCE_DIR}/libs/libcef_dll_wrapper.a
  )

  set_target_properties(nativegui PROPERTIES
      MACOSX_BUNDLE_GUI_IDENTIFIER com.kamerado.native_app
      MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
      MACOSX_BUNDLE_SHORT_VERSION_STRING
          ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
      MACOSX_BUNDLE TRUE
      WIN32_EXECUTABLE TRUE
      RUNTIME_OUTPUT_DIRECTORY
          /home/meganuts/projects/USB-Password-Manager/ExtChrome/app/
  )

  qt_add_executable(testCef
    MANUAL_FINALIZATION
    ${CEF_TEST_SOURCES}

  )

  target_link_libraries(testCef PRIVATE
      Qt${QT_VERSION_MAJOR}::Widgets
      stdc++
      pthread
      X11
      ${CEF_ROOT}/build/libcef_dll_wrapper/libcef_dll_wrapper.a
      ${CEF_ROOT}/Release/libcef.so
  )

  add_definitions(${CEF_CXXFLAGS})
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CEF_LDFLAGS}")

  target_include_directories(testCef PRIVATE
      "${CEF_ROOT}/include"
      "${CEF_ROOT}/include/capi"
  )

  set_target_properties(testCef PROPERTIES
      MACOSX_BUNDLE_GUI_IDENTIFIER com.kamerado.native_app
      MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
      MACOSX_BUNDLE_SHORT_VERSION_STRING
          ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
      MACOSX_BUNDLE TRUE
      WIN32_EXECUTABLE TRUE
      RUNTIME_OUTPUT_DIRECTORY
          /home/meganuts/projects/USB-Password-Manager/tests/core_tests/CefThreadTests/build/
  )

endif()
add_test(
  NAME testCef
  COMMAND $<TARGET_FILE:testCef>
)
if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(nativegui)
endif()
