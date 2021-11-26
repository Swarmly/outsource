include(gtu-versions)
SETUP_VERSION_VARS()
SETUP_GIT_VARS()

set(GTU_TEST_DATA_FOLDER "${CMAKE_SOURCE_DIR}/test_data/")

# Set the name for main default component
set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME ${PROJECT_NAME})

# Set the name for main dev component
set(GTU_INSTALL_DEV_COMPONENT_NAME ${PROJECT_NAME}-dev)

set_property(GLOBAL PROPERTY GTU_LIBRARY_LIST)
set_property(GLOBAL PROPERTY GTU_BINARY_LIST)

function(gtu_get_non_test_binary_list TARGETS)
    get_property(tmp GLOBAL PROPERTY GTU_BINARY_LIST)
    list(FILTER tmp EXCLUDE REGEX ".*test.*")
    set(${TARGETS} ${tmp} PARENT_SCOPE)
endfunction()

function(gtu_get_non_test_library_list TARGETS)
    get_property(tmp GLOBAL PROPERTY GTU_LIBRARY_LIST)
    list(FILTER tmp EXCLUDE REGEX ".*test.*")
    set(${TARGETS} ${tmp} PARENT_SCOPE)
endfunction()

function(_gtu_add_to_target_list TARGET TARGET_LIST_NAME)
    get_property(tmp GLOBAL PROPERTY ${TARGET_LIST_NAME})
    list(APPEND tmp ${TARGET})
    set_property(GLOBAL PROPERTY ${TARGET_LIST_NAME} ${tmp})
endfunction()

function(gtu_add_to_library_list TARGET)
    _gtu_add_to_target_list(${TARGET} GTU_LIBRARY_LIST)
endfunction()

function(gtu_add_to_binary_list TARGET)
    _gtu_add_to_target_list(${TARGET} GTU_BINARY_LIST)
endfunction()

function(gtu_print_var PREFIX VARIABLE_NAME)
    # print if the variable is not empty
    if (DEFINED ${VARIABLE_NAME})
        list(LENGTH ${VARIABLE_NAME} _list_length)
        if (_list_length GREATER 1)
            message(STATUS "${PREFIX}: ${${VARIABLE_NAME}}")
        elseif(NOT ${${VARIABLE_NAME}} STREQUAL "")
            message(STATUS "${PREFIX}: ${${VARIABLE_NAME}}")
        endif()
    endif()
endfunction()

function(gtu_print_start_status_message)
    gtu_print_var("MSYS2" MSYS2_PATH)
    gtu_print_var("MinGW prefix" MINGW_PATH)
    gtu_print_var("MinGW binaries" MINGW_BIN_DIR)
    gtu_print_var("C compiler" CMAKE_C_COMPILER)
    gtu_print_var("C++ compiler" CMAKE_CXX_COMPILER)
    gtu_print_var("ASM compiler" CMAKE_ASM_COMPILER)
    gtu_print_var("C compiler flags" CMAKE_C_FLAGS)
    gtu_print_var("C++ compiler flags" CMAKE_CXX_FLAGS)
    gtu_print_var("Verbose" CMAKE_VERBOSE_MAKEFILE)
    gtu_print_var("CMake module dirs" CMAKE_MODULE_PATH)
    gtu_print_var("OpenCV version" OpenCV_VERSION)
    gtu_print_var("OpenCV libraries" OpenCV_LIBS)
    gtu_print_var("OpenCV include path" OpenCV_INCLUDE_DIRS)
endfunction()

macro(GTU_ADD_DEB_DEPENDENCY_PRJ lib version)
    if("${CPACK_DEBIAN_PACKAGE_DEPENDS}" STREQUAL "")
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "${lib} (>= ${version})")
    else()
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, ${lib} (>= ${version})")
    endif()
endmacro()

macro(GTU_ADD_DEB_DEPENDENCY comp lib version)
    if("${CPACK_DEBIAN_${comp}_PACKAGE_DEPENDS}" STREQUAL "")
        set(CPACK_DEBIAN_${comp}_PACKAGE_DEPENDS "${lib} (>= ${version})")
    else()
        set(CPACK_DEBIAN_${comp}_PACKAGE_DEPENDS "${CPACK_DEBIAN_${comp}_PACKAGE_DEPENDS}, ${lib} (>= ${version})")
    endif()
endmacro()

macro(GTU_DEFINE_META)
    set(COMPONENT_NAME_META ${PROJECT_NAME})
    string(TOUPPER "${COMPONENT_NAME_META}" COMPONENT_NAME_META_UPPER_CASE)
    #Need to install at least one file to be able to create package.
    #Looks like internal issue in the cmake
    file(GLOB_RECURSE MD_FILE ${CMAKE_CURRENT_SOURCE_DIR}/*.md)
    install(FILES ${MD_FILE} DESTINATION ${GTU_INSTALL_PREFIX}/share/${PROJECT_NAME} COMPONENT ${COMPONENT_NAME_META})
endmacro()

function(gtu_define_cpp_sources_filter VARIABLE_NAME SOURCES_DIR_NAME)
    set(_EXTENDER "")
    if (DEFINED SOURCES_DIR_NAME AND NOT ${SOURCES_DIR_NAME} STREQUAL "")
        set(_EXTENDER "${SOURCES_DIR_NAME}/")
    endif()
    set(${VARIABLE_NAME}
            ${CMAKE_CURRENT_SOURCE_DIR}/${_EXTENDER}*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/${_EXTENDER}*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/${_EXTENDER}*.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/${_EXTENDER}*.h
            ${CMAKE_CURRENT_SOURCE_DIR}/${_EXTENDER}*.hpp

            PARENT_SCOPE
            )
endfunction()

function(gtu_add_target TARGET_NAME)
    set(oneValueArgs EXCLUDE_FILES_REGEXP IS_BINARY)
    set(multiValueArgs EXTRA_SOURCES)
    cmake_parse_arguments(MY_INSTALL "${options}" "${oneValueArgs}"
            "${multiValueArgs}" ${ARGN} )
    cmake_parse_arguments(
            _ARG
            "" # options
            "${oneValueArgs}"
            "${multiValueArgs}" ${ARGN}
    )

    if (DEFINED _ARG_IS_BINARY AND _ARG_IS_BINARY)
        # look for sources files inside top level folder
        set(HDRS_FILTERS "")
        gtu_define_cpp_sources_filter(SRCS_FILTERS "")
    else()
        gtu_define_cpp_sources_filter(SRCS_FILTERS "src")
        gtu_define_cpp_sources_filter(HDRS_FILTERS "include")
    endif()

    # look for sources files inside top level folder
    file(GLOB_RECURSE SRCS_AND_HDRS ${HDRS_FILTERS} ${SRCS_FILTERS})

    # exclude files specified in exclude filter
    if(DEFINED _ARG_EXCLUDE_FILES_REGEXP)
        list(FILTER SRCS_AND_HDRS EXCLUDE REGEX ${_ARG_EXCLUDE_FILES_REGEXP})
    endif()

    # add extra sources
    if (DEFINED _ARG_EXTRA_SOURCES)
        list(APPEND SRCS_AND_HDRS ${_ARG_EXTRA_SOURCES})
    endif()

    if (DEFINED _ARG_IS_BINARY AND _ARG_IS_BINARY)
        # add all *.ui and *.qrc files
        file(GLOB_RECURSE RES_AND_UI_FILES
                "${CMAKE_CURRENT_SOURCE_DIR}/*.ui"
                "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc"
        )

        set(SRCS_AND_HDRS_AND_UI_RES ${SRCS_AND_HDRS})
        list(APPEND SRCS_AND_HDRS_AND_UI_RES ${RES_AND_UI_FILES})

        #if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/res/res.qrc")
        #list(APPEND SRCS_AND_HDRS_AND_UI_RES "${CMAKE_CURRENT_SOURCE_DIR}/res/res.qrc")
        #endif()

        add_executable(${TARGET_NAME} ${SRCS_AND_HDRS_AND_UI_RES})
        gtu_add_to_binary_list(${TARGET_NAME})
    else()
       # add all *.ui and *.qrc files
        file(GLOB_RECURSE RES_AND_UI_FILES
                "${CMAKE_CURRENT_SOURCE_DIR}/*.ui"
                "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc"
        )

        set(SRCS_AND_HDRS_AND_UI_RES ${SRCS_AND_HDRS})
        list(APPEND SRCS_AND_HDRS_AND_UI_RES ${RES_AND_UI_FILES})

        add_library(${TARGET_NAME} SHARED ${SRCS_AND_HDRS} ${SRCS_AND_HDRS_AND_UI_RES})
        gtu_add_to_library_list(${TARGET_NAME})
    endif()
endfunction()

# use this function to create a library target
function(gtu_add_library TARGET)
    gtu_add_target(${TARGET} ${ARGN})
endfunction()

# use this function to create a binary target
function(gtu_add_binary TARGET)
    gtu_add_target(${TARGET} IS_BINARY TRUE ${ARGN})
endfunction()

macro(GTU_ADD_INTEGRATION_TEST)
    file(GLOB files "integration_test/*.json")
    if(EXISTS "/software/gtu/bin/gtu-test-runner")
        set(TEST_APP /software/gtu/bin/gtu-test-runner)
    else()
        set(TEST_APP ${EXECUTABLE_OUTPUT_PATH}/gtu-test-runner)
    endif()

    foreach(file ${files})
        message("Project:" ${PROJECT_NAME} " Add integration test with file:" ${file})
        get_filename_component(FILE_NAME "${file}" NAME)
        add_test(${PROJECT_NAME}-integration-test-${FILE_NAME} ${TEST_APP} --gtest_output=xml:${CMAKE_BINARY_DIR}/reports/${FILE_NAME}.xml -c ${file})
    endforeach()
endmacro()

function(GTU_DEFINE_LIB)
    #=================Include section=================
    #include folder should be inthis order. Searching first in the source dir.
    #include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
    #include_directories(${GTU_BUILD_ROOT_PATH}/include)

    #=================Project section=================
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    add_definitions(-DMAVLINK_USE_CXX_NAMESPACE)

    string(TOUPPER ${PROJECT_NAME} UPPER_PROJECT)
    string(REPLACE "-" "_" UPPER_PROJECT ${UPPER_PROJECT})
    add_definitions(-D${UPPER_PROJECT}_LIBRARY)
    message("ADD export definition: " ${UPPER_PROJECT}_LIBRARY)

    include_directories(/opt/ros/melodic/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/gtu-core)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

    gtu_add_library(${PROJECT_NAME} ${ARGN})
    find_package(Qt5 COMPONENTS Core Gui REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui)

    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        PROPERTY Qt5Core_VERSION_MAJOR "${Qt5Core_VERSION_MAJOR}")
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        PROPERTY Qt5Core_VERSION_MINOR "${Qt5Core_VERSION_MAJOR}")

    target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../gtu-core>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include/gtu-core>
        $<BUILD_INTERFACE:${GTU_BUILD_ROOT_PATH}/include>
        $<INSTALL_INTERFACE:${GTU_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${VERSION_STRING} SOVERSION ${VERSION_STRING})

    set(COMPONENT_NAME ${PROJECT_NAME})
    set(COMPONENT_NAME_DEV ${PROJECT_NAME}-dev)

    set(COMPONENT_CONFIG_NAME ${COMPONENT_NAME}Config)

    # install libs
    set(INSTALL_COMPONENT_NAME ${CMAKE_INSTALL_DEFAULT_COMPONENT_NAME})
    if (${PROJECT_NAME} MATCHES "test.*")
        set(INSTALL_COMPONENT_NAME ${GTU_INSTALL_DEV_COMPONENT_NAME})
    endif()

    install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${COMPONENT_CONFIG_NAME}
        DESTINATION ${GTU_INSTALL_LIBDIR}
        COMPONENT ${INSTALL_COMPONENT_NAME}
        LIBRARY DESTINATION ${GTU_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${GTU_INSTALL_LIBDIR}
    )
    # install dev headers
    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${GTU_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        COMPONENT ${GTU_INSTALL_DEV_COMPONENT_NAME}
        FILES_MATCHING PATTERN "*.h*"
    )

    # This makes the project importable from the build directory
    export(TARGETS ${PROJECT_NAME} FILE cmake/${COMPONENT_NAME_DEV}Config.cmake)

    if (NOT ${PROJECT_NAME} STREQUAL gtu-core)
        target_link_libraries(${PROJECT_NAME} PRIVATE gtu-core)
    endif ()

    #One of the solutions there it is copy files. But it produces recompilation depended projects every time.
    #so lets create symlink
    if (NOT WIN32)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND mkdir -p ${GTU_BUILD_ROOT_PATH}/include/${PROJECT_NAME} && ln -f -s ${CMAKE_CURRENT_SOURCE_DIR}/include/* ${GTU_BUILD_ROOT_PATH}/include/${PROJECT_NAME})
    endif()
    GTU_ADD_INTEGRATION_TEST()
endfunction()

macro(GTU_DEFINE_BIN)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    add_definitions(-DMAVLINK_USE_CXX_NAMESPACE)

    #=================Include section=================
    #include folder should be inthis order. Searching first in the source dir.
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../gtu-core)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include/gtu-core)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ui)
    include_directories(${GTU_BUILD_ROOT_PATH}/include)
    include_directories(${CMAKE_SOURCE_DIR})

    #=================Project section=================
    gtu_add_binary(${PROJECT_NAME} ${ARGN})

    set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${VERSION_STRING} SOVERSION ${VERSION_STRING})
    set(COMPONENT_NAME ${PROJECT_NAME})
    find_package(Qt5 COMPONENTS Core REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core gtu-core)
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        PROPERTY Qt5Core_VERSION_MAJOR "${Qt5Core_VERSION_MAJOR}")
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        PROPERTY Qt5Core_VERSION_MINOR "${Qt5Core_VERSION_MAJOR}")

    install(TARGETS ${PROJECT_NAME} DESTINATION ${GTU_INSTALL_BINDIR})
    GTU_ADD_INTEGRATION_TEST()
endmacro()

macro(GTU_DEFINE_TEST)
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTORCC ON)
        set(CMAKE_CXX_STANDARD 17)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        add_definitions(-DMAVLINK_USE_CXX_NAMESPACE)

        add_definitions("-DTEST_DATA_FOLDER=${GTU_TEST_DATA_FOLDER}")

        add_definitions("-DTEST_DATA_FOLDER=${GTU_TEST_DATA_FOLDER}")

        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../gtu-core)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../gtu-core/include)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../gtu-core/src)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
        include_directories(${GTU_BUILD_ROOT_PATH}/include)
        include_directories(${CMAKE_SOURCE_DIR})

        #=================Project section=================
        file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)
        file(GLOB_RECURSE HDRS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.h)


        add_executable(test_${PROJECT_NAME} ${SRCS} ${HDRS})
        set_target_properties(test_${PROJECT_NAME} PROPERTIES VERSION ${VERSION_STRING} SOVERSION ${VERSION_STRING})
        set(COMPONENT_NAME test_${PROJECT_NAME})
        find_package(Qt5 COMPONENTS Core REQUIRED)
        target_link_libraries(test_${PROJECT_NAME} PRIVATE Qt5::Core test-common gtu-core ${PROJECT_NAME})
        
        set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            PROPERTY Qt5Core_VERSION_MAJOR "${Qt5Core_VERSION_MAJOR}")
        set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            PROPERTY Qt5Core_VERSION_MINOR "${Qt5Core_VERSION_MAJOR}")

        install(TARGETS test_${PROJECT_NAME}
                DESTINATION ${GTU_INSTALL_BINDIR}
                COMPONENT ${GTU_INSTALL_DEV_COMPONENT_NAME}
        )

        add_test(test_${PROJECT_NAME} ${EXECUTABLE_OUTPUT_PATH}/test_${PROJECT_NAME} --gtest_output=xml:${CMAKE_BINARY_DIR}/reports/)
endmacro()

macro(GTU_DEFINE_TEST_LIB)
    add_definitions("-DTEST_DATA_FOLDER=${GTU_TEST_DATA_FOLDER}")
    GTU_DEFINE_LIB()
endmacro()
