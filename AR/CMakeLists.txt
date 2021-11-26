cmake_minimum_required (VERSION 3.5)
project (hermes-ar)

# TODO: ideally we should make it work before project() is called.
# Set env var MSYS2_PATH to your system environment
include(cmake/gtu-init.cmake)

# Including custom cmake rules
include(cmake/clang-cpp-dev-tools.cmake)

set(GTU_INSTALL_PREFIX /software/gtu)

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../shared/cmake")
list(APPEND CMAKE_MODULE_PATH /software/gtu/shared/cmake)

#set(DESKTOP_BUILD 1)
set(GTU_LIB_DIR bin)
set(GTU_BIN_DIR bin)
#set(GTU_BUILD_ROOT_PATH ${CMAKE_BINARY_DIR}/..)
get_filename_component(GTU_BUILD_ROOT_PATH "./.." REALPATH BASE_DIR ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${GTU_BUILD_ROOT_PATH}/${GTU_LIB_DIR})
set(EXECUTABLE_OUTPUT_PATH ${GTU_BUILD_ROOT_PATH}/${GTU_BIN_DIR})
link_directories(${LIBRARY_OUTPUT_PATH})
link_directories(${GTU_INSTALL_PREFIX}/${GTU_LIB_DIR})
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH}:./:../lib:/software/gtu/lib:/software/gtu/bin)
#set(CMAKE_INSTALL_RPATH ${GTU_INSTALL_PREFIX}/${GTU_LIB_DIR})
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

set(GTU_INSTALL_INCLUDEDIR ${GTU_INSTALL_PREFIX}/include)
set(GTU_INSTALL_LIBDIR ${GTU_INSTALL_PREFIX}/${GTU_LIB_DIR})
set(GTU_INSTALL_BINDIR ${GTU_INSTALL_PREFIX}/${GTU_BIN_DIR})
set(GTU_INSTALL_CONFIGDIR ${GTU_INSTALL_LIBDIR}/cmake)
set(GTU_INSTALL_SCRIPTDIR ${GTU_INSTALL_PREFIX}/scripts)


option(BUILD_WITH_OPENMP "Use OpenMP" ON)
option(WITH_3D "Enable 3D support" ON)
option(SET_USER_BREAK_AT_STARTUP "Set user wait point in startup (for debug)" OFF)

find_package(PkgConfig)

message("BUILD TYPE IS: " ${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include(gtu-common)

if(NOT DEFINED GTU_VERSION)
    set(GTU_VERSION 0.0.0)
endif()
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(SO_VERSION ${VERSION_MAJOR})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON) # for .moc


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

EXEC_PROGRAM(uname ARGS -r OUTPUT_VARIABLE DESTINATION_KERNEL_MODULES)
#SET(DESTINATION_KERNEL_MODULES "5.4.82-sunxi")

#set(CMAKE_AUTORCC_OPTIONS ON)
#set(CMAKE_AUTOUIC_OPTIONS ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

if(POLICY CMP0063) # visibility. since 3.3
    cmake_policy(SET CMP0063 NEW)
endif()
if(POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()

find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_INSTALL_OPENMP_LIBRARIES TRUE)
    message (STATUS "Found OpenMP")
else(OPENMP_FOUND)
    message (STATUS "Not found OpenMP")
endif()
#if (WITH_3D)
#    # look up for necessary qt3d modules
#    find_package(Qt5 COMPONENTS 3DCore 3DExtras 3DRender 3DInput)
#    if (Qt53DCore_FOUND AND Qt53DExtras_FOUND AND Qt53DRender_FOUND AND Qt53DInput_FOUND)
#       set(HAVE_3D TRUE)
#       add_definitions(-DHAVE_QT3D)
#    else()
#       message (STATUS "Qt 3D was not found! 3D support is diabled.")
#    endif()
#else()
#     message(STATUS "3D support is disabled! To enable 3D support set WITH_3D to ON.")
#endif()

add_definitions("-DGIT_COMMIT_HASH=${GIT_COMMIT_HASH}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Werror -fPIC -Wno-deprecated-declarations -Wall -Wextra -Wno-missing-field-initializers -Wno-error=unused-function -Wno-error=unused-parameter -Wno-error=unused-value -Wstrict-aliasing=3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fPIC -Wno-deprecated-declarations -Wall -Wextra -Wno-missing-field-initializers -Wno-error=unused-function -Wno-error=unused-parameter -Wno-error=unused-value -Wstrict-aliasing=3")

add_compile_options(-fPIC -Wno-deprecated-declarations -Wall -Wextra -Wno-missing-field-initializers -Wno-error=unused-function -Wno-error=unused-parameter -Wno-error=unused-value -Wstrict-aliasing=3)
#add_compile_options(-std=c++14)
#-std=c++14
#add_compile_options(-Werror)

if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.6)
    message("Add compilation flag for version GCC 6 and more")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=implicit-fallthrough")
endif()

install(FILES
    cmake/FindQtAV.cmake cmake/gtu-common.cmake cmake/gtu-versions.cmake
    DESTINATION /software/gtu/shared/cmake
    )


enable_testing()

add_compile_options(-Wno-error)

set(QTAV_MAJOR VERSION_MAJOR)
set(QTAV_MINOR VERSION_MINOR)
set(QTAV_PATCH VERSION_PATCH)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=expansion-to-defined")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=misleading-indentation")

set(HAVE_QT5OPENGL 0)
if(BUILD_QT5OPENGL)
    find_package(Qt5OpenGL)
    if(Qt5OpenGL_FOUND)
        set(HAVE_QT5OPENGL 1)
    endif()
endif()
set(HAVE_QT5OPENGL 1)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
get_target_property(qmake Qt5::qmake LOCATION)
add_subdirectory("libQtAV")

find_package(Qt5Widgets REQUIRED)
get_target_property(qmake Qt5::Widgets LOCATION)
find_package(Qt5OpenGL REQUIRED)
get_target_property(qmake Qt5::OpenGL LOCATION)
add_subdirectory("libQtAVWidgets")

# Add as the last
#add_subdirectory("hermes-gui")
#add_subdirectory("hermes-gcs")
add_subdirectory("gui")

MACRO(CONFIGURE_DOXYGEN_FILE DOXYGEN_CONFIG_FILE FILE_NAME_SUFFIX)
IF(EXISTS ${PROJECT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE})
FILE(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/doxy-${FILE_NAME_SUFFIX}.conf)
FILE(READ ${PROJECT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE} DOXYFILE_CONTENTS)
STRING(REGEX REPLACE ";" "\\\\;" DOXYFILE_CONTENTS "${DOXYFILE_CONTENTS}")
STRING(REGEX REPLACE "\n" ";" DOXYFILE_LINES "${DOXYFILE_CONTENTS}")
LIST(LENGTH DOXYFILE_LINES ROW)
MATH(EXPR ROW "${ROW} - 1")
FOREACH(I RANGE ${ROW})
LIST(GET DOXYFILE_LINES ${I} LINE)
IF(LINE STRGREATER "")
STRING(REGEX MATCH "^[a-zA-Z]([^ ])+" DOXY_PARAM ${LINE})
IF(DEFINED DOXY_${DOXY_PARAM})
STRING(REGEX REPLACE "=([^\n])+" "= ${DOXY_${DOXY_PARAM}}" LINE ${LINE})
ENDIF(DEFINED DOXY_${DOXY_PARAM})
ENDIF()
FILE(APPEND ${CMAKE_CURRENT_BINARY_DIR}/doxy-${FILE_NAME_SUFFIX}.conf "${LINE}\n")
ENDFOREACH()
ELSE()
MESSAGE(SEND_ERROR "Doxygen configuration file '${DOXYGEN_CONFIG_FILE}' not found. Documentation will not be generated")
ENDIF()
ENDMACRO(CONFIGURE_DOXYGEN_FILE)

MACRO(ADD_DOCUMENTATION TARGET DOXYGEN_CONFIG_FILE)
FIND_PACKAGE(Doxygen QUIET)
IF(DOXYGEN_FOUND)
CONFIGURE_DOXYGEN_FILE(${DOXYGEN_CONFIG_FILE} ${TARGET})
ADD_CUSTOM_TARGET(${TARGET} COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxy-${TARGET}.conf)
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/../shared/gtu/docs
    DESTINATION ${GTU_INSTALL_PREFIX}/shared
    FILES_MATCHING PATTERN "*.*"
    )
ELSE(DOXYGEN_FOUND)
MESSAGE(STATUS "Doxygen not found. Documentation will not be generated")
ADD_CUSTOM_TARGET(${TARGET} COMMAND echo WARNING! Not installed Doxygen documentation system.)
ENDIF(DOXYGEN_FOUND)
ENDMACRO(ADD_DOCUMENTATION)

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/../shared/gtu/docs)
SET(DOXY_INPUT ${PROJECT_SOURCE_DIR})
SET(DOXY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../shared/gtu/docs)
SET(DOXY_PROJECT_NUMBER ${GTU_VERSION})

ADD_DOCUMENTATION(doc Doxyfile)

add_subdirectory("packaging")
