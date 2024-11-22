#!/bin/bash

# Download Qt installer
wget https://download.qt.io/official_releases/online_installers/qt-unified-linux-x64-online.run

# Make installer executable
chmod +x qt-unified-linux-x64-online.run

# Run installer with parameters
./qt-unified-linux-x64-online.run \
  --root "$HOME/Qt" \
  --accept-licenses \
  --default-answer \
  --confirm-command \
  install \
  qt6-full-dev \
  qt.qt6.680.linux_gcc_64 \
  qt.qt6.680.addons.qtwebsockets

# Cleanup
rm qt-unified-linux-x64-online.run
