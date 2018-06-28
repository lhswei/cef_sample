cmake_minimum_required(VERSION 2.8)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Project name.
# TODO: Change this line to match your project name when you copy this file.
project(demon)

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

if(CMAKE_CL_64)    #CMAKE的内建变量，如果是true，就说明编译器的64位的，自然可以编译64bit的程序
  set(ADDRESS_MODEL 64)
  set(NODE_TARGET x64)
else()
  set(ADDRESS_MODEL 32)
  set(NODE_TARGET x86)
endif()

#
# CEF_ROOT setup.
# This variable must be set to locate the binary distribution.
# TODO: Choose one of the below examples and comment out the rest.
#

# Example 1: The current directory contains both the complete binary
#            distribution and your project.
# A. Comment in these lines:
#
set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_ROOT}/cmake")

set(LUA_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/lua_source" )
set(LUA_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/lua_source/include" )

set(CEF_UI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/cef_ui" )
# Execute FindCEF.cmake which must exist in CMAKE_MODULE_PATH.
find_package(CEF REQUIRED)

# Include application targets.
# Comes from the <target>/CMakeLists.txt file in the current directory.
# TODO: Change these lines to match your project target when you copy this file.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test")
  add_subdirectory(test)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/writesoul" )
  add_subdirectory(writesoul)
endif()

# Display configuration settings.
PRINT_CEF_CONFIG()

