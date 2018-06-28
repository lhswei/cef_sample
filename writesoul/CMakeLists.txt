project(write_soul)
cmake_minimum_required(VERSION 2.8)

set(WRITESOULD_TARGET_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>")

include_directories(
	${_CEF_ROOT}/
	${_CEF_ROOT}/include
	${_CEF_ROOT}/include/cef_headers
	${_CEF_ROOT}/include/dx9sdk
	${_CEF_ROOT}/lua_source/include
	${CEF_UI_ROOT})

set(LIB_DIR ${_CEF_ROOT}/lib ${_CEF_ROOT}/lib/dx9sdk/${NODE_TARGET})
link_directories("${LIB_DIR}")

find_library(libcef_dll_wrapper libcef_dll_wrapper ${_CEF_ROOT}/lib NO_DEFAULT_PATH)
ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")

SET_TARGET_OUT_DIR(WRITESOUL_TARGET_OUT_DIR)

set(WRITESOUL_TARGET "write_soul")

file(
    GLOB_RECURSE 
    LUA_ALL_LIST 
	"${LUA_ROOT}/*.cpp" 
	"${LUA_ROOT}/*.c" 
	"${LUA_ROOT}/*.h" 
    "${LUA_ROOT}/*.hpp" 
)

file(
    GLOB_RECURSE 
    CEF_UI_LIST 
    "${CEF_UI_ROOT}/*.cpp" 
    "${CEF_UI_ROOT}/*.c" 
    "${CEF_UI_ROOT}/*.h" 
    "${CEF_UI_ROOT}/*.hpp" 
)

file(
    GLOB_RECURSE 
    WRITESOULD_SRC_LIST 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

set(WRITE_ALL_LIST ${LUA_ALL_LIST} ${WRITESOULD_SRC_LIST} ${CEF_UI_LIST})

set(WRITE_ROOT_SRC 
"${CMAKE_CURRENT_SOURCE_DIR}/GameApp.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp")

set(WRITE_ROOT_HEAD 
"${CMAKE_CURRENT_SOURCE_DIR}/GameApp.h"
"${CMAKE_CURRENT_SOURCE_DIR}/utils.h")

set(WRITE_D3D_SRC
"${CMAKE_CURRENT_SOURCE_DIR}/D3DApi.h"
"${CMAKE_CURRENT_SOURCE_DIR}/D3DImpl.cpp"
)

set(WRITE_UI_SRC
"${CMAKE_CURRENT_SOURCE_DIR}/GObject.h"
"${CMAKE_CURRENT_SOURCE_DIR}/GObject.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/InterfaceWorld.h"
"${CMAKE_CURRENT_SOURCE_DIR}/InterfaceWorld.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/create_ui.lua"
)

add_executable(${WRITESOUL_TARGET} WIN32 ${WRITE_ALL_LIST})
SET_EXECUTABLE_TARGET_PROPERTIES(${WRITESOUL_TARGET})
target_link_libraries(${WRITESOUL_TARGET} libcef_lib ${libcef_dll_wrapper})

file(
    GLOB 
    LUA_INCLUDE_LIST 
	"${LUA_INCLUDE}/*.h" 
	"${LUA_INCLUDE}/*.hpp" 
)

file(
    GLOB_RECURSE 
    LUA_ALL_SRC_LIST 
	"${LUA_ROOT}/*.cpp" 
	"${LUA_ROOT}/*.c" 
)

source_group("src" FILES ${WRITE_ROOT_SRC}) 
source_group("headers" FILES ${WRITE_ROOT_HEAD} ) 
source_group("headers\\lua" FILES ${LUA_INCLUDE_LIST}) 
source_group("src\\cef_ui" FILES ${CEF_UI_LIST}) 
source_group("src\\d3d" FILES ${WRITE_D3D_SRC}) 
source_group("src\\ui" FILES ${WRITE_UI_SRC}) 
source_group("src\\lua" FILES ${LUA_ALL_LIST}) 

# Add the custom manifest files to the executable.
ADD_WINDOWS_MANIFEST("${WRITESOUL_TARGET_OUT_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" "${WRITESOUL_TARGET}" "exe")

# Copy binary and resource files to the target output directory.
COPY_FILES("${WRITESOUL_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${WRITESOULD_TARGET_OUT_DIR}")
COPY_FILES("${WRITESOUL_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${WRITESOULD_TARGET_OUT_DIR}")

