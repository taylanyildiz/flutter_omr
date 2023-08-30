# CMake Version
cmake_minimum_required(VERSION 3.4.1)

# C++ Version 11.0
set (CMAKE_CXX_STANDARD 11)

# Project
project(flutter_omr_library)

# Open-CV directory
include_directories(../include)

# Flutter Orm C++ File
add_library(flutter_omr SHARED
  "flutter_omr.cpp"
)

# Open-CV Imported
add_library(lib_opencv SHARED IMPORTED)

# Andriod ABI
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../android/src/main/jniLibs/${ANDROID_ABI}/libopencv_java4.so)

# Flutter ORM Library
set_target_properties(flutter_omr PROPERTIES
  OUTPUT_NAME "flutter_omr"
)

# Flutter ORM Dart
target_compile_definitions(flutter_omr PUBLIC DART_SHARED_LIB)

# Android Logger
find_library(log-lib log)
target_link_libraries(flutter_omr lib_opencv ${log-lib})