# Must be loaded via FindCLIENT.cmake.
if(NOT DEFINED _CLIENT_ROOT_EXPLICIT)
  message(FATAL_ERROR "Use find_package(CLIENT) to load this file.")
endif()

# Print the current CLIENT configuration.
macro(PRINT_CLIENT_CONFIG)
  message(STATUS "*** CLIENT CONFIGURATION SETTINGS ***")
  message(STATUS "Generator:                    ${CMAKE_GENERATOR}")
  message(STATUS "Platform:                     ${CMAKE_SYSTEM_NAME}")
  message(STATUS "Project architecture:         ${PROJECT_ARCH}")

endmacro()

# Determine the target output directory based on platform and generator.
macro(SET_TARGET_OUT_DIR current_target_out_dir)
  set(${current_target_out_dir} "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>")
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
