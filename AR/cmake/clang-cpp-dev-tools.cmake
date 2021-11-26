# Additional targets to perform clang-format/clang-tidy
# Get all project files
file(GLOB_RECURSE
        ALL_CPP_SOURCE_FILES
        *.[chi]pp *.[chi]xx *.cc *.hh *.ii *.[CHI]
)

# Adding clang-format target if executable is found
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    add_custom_target(
            clang-format
            COMMAND clang-format
            -i
            -style=file
            ${ALL_CPP_SOURCE_FILES}
    )
endif()

# Adding clang-tidy target if executable is found
find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
    add_custom_target(
            clang-tidy
            COMMAND clang-tidy
            ${ALL_CPP_SOURCE_FILES}
            -config=''
            --
            -std=c++14
            ${INCLUDE_DIRECTORIES}
    )
endif()