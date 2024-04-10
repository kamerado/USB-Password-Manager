#!/bin/sh

g++ src/main.cpp src/core/DatabaseManager.cpp -l sqlite3 -o main && ./main