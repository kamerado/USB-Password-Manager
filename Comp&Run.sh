#!/bin/sh

g++ src/main.cpp src/core/DatabaseManager.cpp -l sqlcipher -o main && ./main