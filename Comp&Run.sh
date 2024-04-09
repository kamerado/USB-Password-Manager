#!/bin/sh

g++ src/main.cpp src/core/DatabaseManager.cpp -lsqlite3 -o main && ./main