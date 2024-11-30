# USB Password Manager

A secure, cross-platform password manager that can be run from a USB drive. Built with Qt 6 and modern C++.

## Features

- 🔒 Strong encryption using ChaCha20-Poly1305
- 💾 Portable - runs from USB drive
- 🖥️ Cross-platform (Windows/Linux)
- 🎨 Modern Qt6 GUI
- 📱 Chrome extension support
- 🔑 Master password protection
- ⚡ Fast and lightweight

## Quick Start

1. Clone the repository:
```bash
git clone https://github.com/kamerado/USB-Password-Manager.git
Navigate to build directory:
Run the build script:
On Linux:

On Windows:

The build script will:

Install Qt 6.8.0 and dependencies if needed
Configure CMake build
Build the application
Launch the password manager
Requirements
Git
CMake 3.29+
C++17 compatible compiler
Qt 6.8.0+ (auto-installed by build scripts)
License
This project is licensed under the MIT License - see the LICENSE file for details.

Contributing
Contributions are welcome! Please feel free to submit pull requests.

Security
Uses ChaCha20-Poly1305 for encryption
Master password never stored, only hash
Auto-lock functionality
Clipboard timeout protection