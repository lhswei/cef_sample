project(uitest)
cmake_minimum_required(VERSION 2.8)

set(UITEST_TARGET "uitest")



file(GLOB_RECURSE UITEST_SRC_LIST 
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

file(GLOB_RECURSE CEF_UI_LIST 
  "${CEF_UI_ROOT}/*.cpp" 
  "${CEF_UI_ROOT}/*.c" 
  "${CEF_UI_ROOT}/*.h" 
  "${CEF_UI_ROOT}/*.hpp" 
)

file(GLOB_RECURSE LUA_ALL_LIST 
  "${LUA_ROOT}/*.cpp" 
  "${LUA_ROOT}/*.c" 
  "${LUA_ROOT}/*.h" 
  "${LUA_ROOT}/*.hpp" 
)

file(GLOB_RECURSE APP_ALL_LIST 
  "${APP_ROOT}/*.cpp" 
  "${APP_ROOT}/*.c" 
  "${APP_ROOT}/*.h" 
  "${APP_ROOT}/*.hpp" 
  "${_CLIENT_ROOT}/utils.cpp"
  "${_CLIENT_ROOT}/utils.h"
)

set(UITEST_ROOT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
set(UITEST_ROOT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/config.h")

include_directories(
	${_CLIENT_ROOT}/
	${_CLIENT_ROOT}/include
	${_CLIENT_ROOT}/include/cef_headers
	${_CLIENT_ROOT}/include/dx9sdk
	${_CLIENT_ROOT}/include/dx9sdk/include
	${_CLIENT_ROOT}/src/lua_source/include
	${_CLIENT_ROOT}/src/app
  ${CEF_UI_ROOT}
)

# Determine the target output directory.
SET_TARGET_OUT_DIR(UITEST_TARGET_OUT_DIR)

set(UITEST_ALL_LIST ${LUA_ALL_LIST} ${UITEST_SRC_LIST} ${CEF_UI_LIST} ${APP_ALL_LIST})
## test
add_executable(${UITEST_TARGET} WIN32 ${UITEST_ALL_LIST})


source_group("src" FILES ${UITEST_ROOT_SRC})
source_group("headers" FILES ${UITEST_ROOT_HEADERS}) 
source_group("headers\\lua" FILES ${LUA_INCLUDE_LIST}) 
source_group("src\\cef_ui" FILES ${CEF_UI_LIST}) 
source_group("src\\lua" FILES ${LUA_ALL_LIST}) 
source_group("src\\app" FILES ${APP_ALL_LIST}) 

# Add the custom manifest files to the executable.
ADD_WINDOWS_MANIFEST("${UITEST_TARGET_OUT_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" "${UITEST_TARGET}" "exe")
