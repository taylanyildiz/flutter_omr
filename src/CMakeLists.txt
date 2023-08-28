# CMake Version
cmake_minimum_required(VERSION 3.4.1)

# C++ Version 11.0
set (CMAKE_CXX_STANDARD 11)

# Project
project(flutter_orm_library)

# Open-CV directory
include_directories(../include)

# Flutter Orm C++ File
add_library(flutter_orm SHARED
  "flutter_orm.cpp"
)

# Open-CV Imported
add_library(lib_opencv SHARED IMPORTED)

# Andriod ABI
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../android/src/main/jniLibs/${ANDROID_ABI}/libopencv_java4.so)

# Flutter ORM Library
set_target_properties(flutter_orm PROPERTIES
  OUTPUT_NAME "flutter_orm"
)

# Flutter ORM Dart
target_compile_definitions(flutter_orm PUBLIC DART_SHARED_LIB)

# Android Logger
find_library(log-lib log)
target_link_libraries(flutter_orm lib_opencv ${log-lib})