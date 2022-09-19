#!/bin/bash

BUILD_DIR=build

mkdir -p $BUILD_DIR
cc src/main.c -I include/ -o Shell
mv Shell $BUILD_DIR
