project(cef_test)
cmake_minimum_required(VERSION 2.8)

include_directories(
  ../
  ../include
  ../include/cef_headers
  ../cef_ui)

find_library(libcef_dll_wrapper libcef_dll_wrapper ${_CEF_ROOT}/lib NO_DEFAULT_PATH)

set(CEF_TARGET "cef_test")

file(
    GLOB 
    SRC_LIST 
    "../cef_ui/*.cpp" 
    "../cef_ui/*.c" 
    "../cef_ui/*.hpp" 
    "../cef_ui/*.h" 
    "./main.cpp"
) 

# Logical target used to link the libcef library.
# message(STATUS "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
# Determine the target output directory.
SET_TARGET_OUT_DIR(CEF_TARGET_OUT_DIR)

## test
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
add_executable(${CEF_TARGET} WIN32 ${SRC_LIST})
SET_EXECUTABLE_TARGET_PROPERTIES(${CEF_TARGET})

target_link_libraries(${CEF_TARGET} libcef_lib ${libcef_dll_wrapper})

set(REG_SRC "../cef_ui/") 
set(REG_HDR "../cef_ui/") 
set(REG_EXT1 "[^/]*([.]cpp|[.]c)$") 
set(REG_EXT2 "[^/]*([.]h|[.]hpp)$") 

# source_group("src" REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/${REG_EXT}") 
source_group("src" FILES "./main.cpp") 
source_group("src\\sources" REGULAR_EXPRESSION "${REG_SRC}${REG_EXT1}") 
source_group("src\\headers" REGULAR_EXPRESSION "${REG_HDR}${REG_EXT2}") 

# Add the custom manifest files to the executable.
ADD_WINDOWS_MANIFEST("${CEF_TARGET_OUT_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" "${CEF_TARGET}" "exe")

# Copy binary and resource files to the target output directory.
COPY_FILES("${CEF_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${CEF_TARGET_OUT_DIR}")
COPY_FILES("${CEF_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${CEF_TARGET_OUT_DIR}")


