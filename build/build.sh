#!/bin/bash

# "/usr/local/bin/cmake" --build /home/noname/projects/USB-Password-Manager/build/Desktop_Qt_6_8_0-Debug --target all && ./nativegui

QT_DIR="$HOME/Qt/6.8.0/gcc_64"
if [ ! -d "$QT_DIR" ]; then
  echo "Error: Qt directory not found"
  exit 1
fi
# if not exist "C:\Users\user\Qt\Tools\mingw1310_64" (
#     echo Error: MinGW directory not found
#     exit /b 1
# )

# Set Qt and MinGW paths
export PATH="$PATH:$QT_DIR/bin"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$QT_DIR/lib"
PROJECT_DIR="$HOME/projects/USB-Password-Manager"
BUILD_DIR="$PROJECT_DIR/build/Desktop_Qt_6_8_0-Debug"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

# Configure CMake with Qt settings
cd "$BUILD_DIR"
cmake -G "Unix Makefiles" \
  -DCMAKE_PREFIX_PATH="$QT_DIR" \
  -DCMAKE_C_COMPILER="/usr/bin/gcc" \
  -DCMAKE_CXX_COMPILER="/usr/bin/g++" \
  -DCMAKE_BUILD_TYPE=Debug \
  "$PROJECT_DIR"

if [ $? -ne 0 ]; then
  echo "CMake configuration failed"
  exit 1
fi

echo "Build directory setup complete at:"
echo "$BUILD_DIR"
# Set working directory to build output
cd "$(dirname "$0")"

# Build project
echo Building project...
cmake --build "$HOME/projects/USB-Password-Manager/build/Desktop_Qt_6_8_0-Debug" --target all
if [ ! -f "$PROJECT_DIR/nativegui" ]; then
  echo "Build failed - Check CMake output for details"
  exit 1
fi

# Deploy Qt dependencies
# echo "Deploying Qt dependencies..."
# if [ ! -f "nativegui" ]; then
#   echo "Error: nativegui.exe not found"
#   exit 1
# fi

# Verify critical DLLs
# if [ ! -f "libspdlog.so" ]; then
#   echo "Error: libspdlog.so missing after deployment"
#   exit 1
# fi

# "$QT_DIR/bin/linuxdeployqt" "nativegui" -qmdir=.
# if [$? -ne 0]; then
#   echo "Qt deployment failed"
#   exit 1
# fi

echo "Build and deployment completed successfully"

# Run the application
echo "Starting application..."
./nativegui &
if [ $? -ne 0 ]; then
  echo "Failed to start application"
  exit 1
fi
