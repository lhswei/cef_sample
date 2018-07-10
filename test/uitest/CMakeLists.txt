project(uitest)
cmake_minimum_required(VERSION 2.8)

set(UITEST_TARGET "uitest")

file(GLOB SRC_LIST
"./main.cpp"
)

# Determine the target output directory.
SET_TARGET_OUT_DIR(UITEST_TARGET_OUT_DIR)

## test
add_executable(${UITEST_TARGET} WIN32 ${SRC_LIST})
SET_EXECUTABLE_TARGET_PROPERTIES(${UITEST_TARGET})

# Add the custom manifest files to the executable.
ADD_WINDOWS_MANIFEST("${UITEST_TARGET_OUT_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" "${UITEST_TARGET}" "exe")
