cmake_minimum_required(VERSION 3.10)  # Minimum CMake version required

# Project name
project(USB-PASSWORD-MANAGER)

# Add executable target
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/core/EncryptionUtil.cpp src/core/EncryptionUtil.h)

target_link_libraries(${CMAKE_PROJECT_NAME} crypto++)