# Find the CLIENT binary distribution root directory.
set(_CLIENT_ROOT "")
if(CLIENT_ROOT AND IS_DIRECTORY "${CLIENT_ROOT}")
  set(_CLIENT_ROOT "${CLIENT_ROOT}")
  set(_CLIENT_ROOT_EXPLICIT 1)
else()
  set(_ENV_CLIENT_ROOT "")
  if(DEFINED ENV{CLIENT_ROOT})
    file(TO_CMAKE_PATH "$ENV{CLIENT_ROOT}" _ENV_CLIENT_ROOT)
  endif()
  if(_ENV_CLIENT_ROOT AND IS_DIRECTORY "${_ENV_CLIENT_ROOT}")
    set(_CLIENT_ROOT "${_ENV_CLIENT_ROOT}")
    set(_CLIENT_ROOT_EXPLICIT 1)
  endif()
  unset(_ENV_CLIENT_ROOT)
endif()

if(NOT DEFINED _CLIENT_ROOT_EXPLICIT)
  message(FATAL_ERROR "Must specify a CLIENT_ROOT value via CMake or environment variable.")
endif()

if(NOT IS_DIRECTORY "${_CLIENT_ROOT}/cmake")
  message(FATAL_ERROR "No CMake bootstrap found for CLIENT binary distribution at: ${CLIENT_ROOT}.")
endif()

# Execute additional cmake files from the CLIENT binary distribution.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${_CLIENT_ROOT}/cmake")
include("clien_variables")
include("clien_macros")
