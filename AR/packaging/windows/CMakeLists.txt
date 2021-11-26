add_custom_target(
        ${PROJECT_NAME}-package
        COMMAND
        "${ENV_EXECUTABLE}" "MSYSTEM=MINGW64" "${BASH_EXECUTABLE}" "-l" "${CMAKE_CURRENT_SOURCE_DIR}/package.sh"

        "${CMAKE_SOURCE_DIR}" # project dir
        "${GTU_BUILD_ROOT_PATH}/packages" # packaging root dir
        "${EXECUTABLE_OUTPUT_PATH}" # bin dir
        "${MINGW_BIN_DIR}" # MINGW dir
        "hermes-${PROJECT_VERSION}-win64" # package name
)
# Add dependencies to build all registered targets before packaging
gtu_get_non_test_library_list(non_test_libraries)
message("${PROJECT_NAME}-package top level library dependencies: ${non_test_libraries}")

gtu_get_non_test_binary_list(non_test_binaries)
message("${PROJECT_NAME}-package top level binary dependencies: ${non_test_binaries}")

add_dependencies(${PROJECT_NAME}-package ${non_test_libraries})
add_dependencies(${PROJECT_NAME}-package ${non_test_binaries})