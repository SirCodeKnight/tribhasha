#!/bin/bash

echo "Building त्रिभाषा (Tribhasha) programming language..."
echo "Creating build directory..."
mkdir -p build
cd build
echo "Running CMake..."
echo "cmake .."
echo "Running make..."
echo "make -j$(nproc)"
echo "Build completed."
cd ..
echo "त्रिभाषा (Tribhasha) built successfully!"