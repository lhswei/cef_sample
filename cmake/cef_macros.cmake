# Must be loaded via FindCEF.cmake.
if(NOT DEFINED _CEF_ROOT_EXPLICIT)
  message(FATAL_ERROR "Use find_package(CEF) to load this file.")
endif()

# Print the current CEF configuration.
macro(PRINT_CEF_CONFIG)
  message(STATUS "*** CEF CONFIGURATION SETTINGS ***")
  message(STATUS "Generator:                    ${CMAKE_GENERATOR}")
  message(STATUS "Platform:                     ${CMAKE_SYSTEM_NAME}")
  message(STATUS "Project architecture:         ${PROJECT_ARCH}")

  message(STATUS "Binary distribution root:     ${_CEF_ROOT}")

  set(_libraries ${CEF_STANDARD_LIBS})
  message(STATUS "Standard libraries:           ${_libraries}")

  message(STATUS "Compile defines:              ${CEF_COMPILER_DEFINES}")
  message(STATUS "Compile defines (Debug):      ${CEF_COMPILER_DEFINES_DEBUG}")
  message(STATUS "Compile defines (Release):    ${CEF_COMPILER_DEFINES_RELEASE}")
  message(STATUS "C compile flags:              ${CEF_COMPILER_FLAGS} ${CEF_C_COMPILER_FLAGS}")
  message(STATUS "C compile flags (Debug):      ${CEF_COMPILER_FLAGS_DEBUG} ${CEF_C_COMPILER_FLAGS_DEBUG}")
  message(STATUS "C compile flags (Release):    ${CEF_COMPILER_FLAGS_RELEASE} ${CEF_C_COMPILER_FLAGS_RELEASE}")
  message(STATUS "C++ compile flags:            ${CEF_COMPILER_FLAGS} ${CEF_CXX_COMPILER_FLAGS}")
  message(STATUS "C++ compile flags (Debug):    ${CEF_COMPILER_FLAGS_DEBUG} ${CEF_CXX_COMPILER_FLAGS_DEBUG}")
  message(STATUS "C++ compile flags (Release):  ${CEF_COMPILER_FLAGS_RELEASE} ${CEF_CXX_COMPILER_FLAGS_RELEASE}")
  message(STATUS "Exe link flags:               ${CEF_LINKER_FLAGS} ${CEF_EXE_LINKER_FLAGS}")
  message(STATUS "Exe link flags (Debug):       ${CEF_LINKER_FLAGS_DEBUG} ${CEF_EXE_LINKER_FLAGS_DEBUG}")
  message(STATUS "Exe link flags (Release):     ${CEF_LINKER_FLAGS_RELEASE} ${CEF_EXE_LINKER_FLAGS_RELEASE}")
  message(STATUS "Shared link flags:            ${CEF_LINKER_FLAGS} ${CEF_SHARED_LINKER_FLAGS}")
  message(STATUS "Shared link flags (Debug):    ${CEF_LINKER_FLAGS_DEBUG} ${CEF_SHARED_LINKER_FLAGS_DEBUG}")
  message(STATUS "Shared link flags (Release):  ${CEF_LINKER_FLAGS_RELEASE} ${CEF_SHARED_LINKER_FLAGS_RELEASE}")
  message(STATUS "CEF Binary files:             ${CEF_BINARY_FILES}")
  message(STATUS "CEF Resource files:           ${CEF_RESOURCE_FILES}")
endmacro()

# Append platform specific sources to a list of sources.
macro(APPEND_PLATFORM_SOURCES name_of_list)
  if(${name_of_list}_WINDOWS)
    list(APPEND ${name_of_list} ${${name_of_list}_WINDOWS})
  endif()
endmacro()

# Determine the target output directory based on platform and generator.
macro(SET_TARGET_OUT_DIR current_target_out_dir)
  set(${current_target_out_dir} "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>")
endmacro()

# Copy a list of files from one directory to another. Relative files paths are maintained.
# The path component of the source |file_list| will be removed.
macro(COPY_FILES target file_list source_dir target_dir)
  foreach(FILENAME ${file_list})
    set(source_file ${source_dir}/${FILENAME})
    get_filename_component(target_name ${FILENAME} NAME)
    set(target_file ${target_dir}/${target_name})

    string(FIND ${source_file} "$<CONFIGURATION>" _pos)
    if(NOT ${_pos} EQUAL -1)
      # Must test with an actual configuration directory.
      string(REPLACE "$<CONFIGURATION>" "Release" existing_source_file ${source_file})
      if(NOT EXISTS ${existing_source_file})
        string(REPLACE "$<CONFIGURATION>" "Debug" existing_source_file ${source_file})
      endif()
    else()
      set(existing_source_file ${source_file})
    endif()

    if(IS_DIRECTORY ${existing_source_file})
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${source_file}" "${target_file}"
        VERBATIM
        )
    else()
      add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${source_file}" "${target_file}"
        VERBATIM
        )
    endif()
  endforeach()
endmacro()

# Add custom manifest files to an executable target.
macro(ADD_WINDOWS_MANIFEST target_out_dir manifest_path target extension)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND "mt.exe" -nologo
            -manifest \"${manifest_path}/${target}.${extension}.manifest\" \"${manifest_path}/compatibility.manifest\"
            -outputresource:"${target_out_dir}/${target}.${extension}"\;\#1
    COMMENT "Adding manifest..."
    )
endmacro()

# Add a logical target that can be used to link the specified libraries into an
# executable target.
macro(ADD_LOGICAL_TARGET target debug_lib release_lib)
  add_library(${target} ${CEF_LIBTYPE} IMPORTED)
  set_target_properties(${target} PROPERTIES
    IMPORTED_LOCATION "${release_lib}"
    IMPORTED_LOCATION_DEBUG "${debug_lib}"
    IMPORTED_LOCATION_RELEASE "${release_lib}"
    )
endmacro()

# Set common target properties. Use SET_LIBRARY_TARGET_PROPERTIES() or
# SET_EXECUTABLE_TARGET_PROPERTIES() instead of calling this macro directly.
macro(SET_COMMON_TARGET_PROPERTIES target)
  # Compile flags.
  target_compile_options(${target} PRIVATE ${CEF_COMPILER_FLAGS} ${CEF_CXX_COMPILER_FLAGS})
  target_compile_options(${target} PRIVATE $<$<CONFIG:Debug>:${CEF_COMPILER_FLAGS_DEBUG} ${CEF_CXX_COMPILER_FLAGS_DEBUG}>)
  target_compile_options(${target} PRIVATE $<$<CONFIG:Release>:${CEF_COMPILER_FLAGS_RELEASE} ${CEF_CXX_COMPILER_FLAGS_RELEASE}>)

  # Compile definitions.
  target_compile_definitions(${target} PRIVATE ${CEF_COMPILER_DEFINES})
  target_compile_definitions(${target} PRIVATE $<$<CONFIG:Debug>:${CEF_COMPILER_DEFINES_DEBUG}>)
  target_compile_definitions(${target} PRIVATE $<$<CONFIG:Release>:${CEF_COMPILER_DEFINES_RELEASE}>)

  # Include directories.
  target_include_directories(${target} PRIVATE ${CEF_INCLUDE_PATH})

  # Linker flags.
  if(CEF_LINKER_FLAGS)
    string(REPLACE ";" " " _flags_str "${CEF_LINKER_FLAGS}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS ${_flags_str})
  endif()
  if(CEF_LINKER_FLAGS_DEBUG)
    string(REPLACE ";" " " _flags_str "${CEF_LINKER_FLAGS_DEBUG}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS_DEBUG ${_flags_str})
  endif()
  if(CEF_LINKER_FLAGS_RELEASE)
    string(REPLACE ";" " " _flags_str "${CEF_LINKER_FLAGS_RELEASE}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS_RELEASE ${_flags_str})
  endif()
endmacro()

# Set library-specific properties.
macro(SET_LIBRARY_TARGET_PROPERTIES target)
  SET_COMMON_TARGET_PROPERTIES(${target})

  # Shared library linker flags.
  if(CEF_SHARED_LINKER_FLAGS)
    string(REPLACE ";" " " _flags_str "${CEF_SHARED_LINKER_FLAGS}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS ${_flags_str})
  endif()
  if(CEF_SHARED_LINKER_FLAGS_DEBUG)
    string(REPLACE ";" " " _flags_str "${CEF_SHARED_LINKER_FLAGS_DEBUG}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS_DEBUG ${_flags_str})
  endif()
  if(CEF_SHARED_LINKER_FLAGS_RELEASE)
    string(REPLACE ";" " " _flags_str "${CEF_SHARED_LINKER_FLAGS_RELEASE}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS_RELEASE ${_flags_str})
  endif()
endmacro()


# Set executable-specific properties.
macro(SET_EXECUTABLE_TARGET_PROPERTIES target)
  SET_COMMON_TARGET_PROPERTIES(${target})

  # Executable linker flags.
  if(CEF_EXE_LINKER_FLAGS)
    string(REPLACE ";" " " _flags_str "${CEF_EXE_LINKER_FLAGS}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS ${_flags_str})
  endif()
  if(CEF_EXE_LINKER_FLAGS_DEBUG)
    string(REPLACE ";" " " _flags_str "${CEF_EXE_LINKER_FLAGS_DEBUG}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS_DEBUG ${_flags_str})
  endif()
  if(CEF_EXE_LINKER_FLAGS_RELEASE)
    string(REPLACE ";" " " _flags_str "${CEF_EXE_LINKER_FLAGS_RELEASE}")
    set_property(TARGET ${target} PROPERTY LINK_FLAGS_RELEASE ${_flags_str})
  endif()
endmacro()

macro(source_group_by_dir s_files)
  set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
  foreach(sgbd_file ${${s_files}})
      string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
      string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
      string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
      string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
      if(sgbd_nogroup)
          set(sgbd_group_name "\\")
      endif(sgbd_nogroup)
      source_group(${sgbd_group_name} FILES ${sgbd_file})
  endforeach(sgbd_file)
endmacro(source_group_by_dir)
