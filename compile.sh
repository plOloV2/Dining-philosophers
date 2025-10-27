#!/bin/bash

DEBUG=false
RELEASE=false

while [[ $# -gt 0 ]]; do
    case "$1" in
        -d|--debug)
            DEBUG=true
            shift ;;
        -r|--release)
            RELEASE=true
            shift ;;
        *)
            echo "Unknown option: $1"
            exit 1 ;;
    esac
done

mkdir -p build
cd build
rm -rf *

if $DEBUG; then
    cmake -DCMAKE_BUILD_TYPE=Debug ..
elif $RELEASE; then
    cmake -DCMAKE_BUILD_TYPE=Release ..
else 
    cmake ..
fi

make -j$(nproc)

mv dinning_phi ..
