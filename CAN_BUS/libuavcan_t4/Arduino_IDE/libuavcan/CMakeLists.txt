#
# Copyright (C) 2014 Pavel Kirienko <pavel.kirienko@gmail.com>
#

cmake_minimum_required(VERSION 2.8.11)

project(uavcan C CXX)

#
# Build options
#
# options are listed in a table format below
set(opts
    # name: type: default value: string options list : description
    "CMAKE_BUILD_TYPE:STRING:RelWithDebInfo:Debug Release RelWithDebInfo MinSizeRel:Build type."
    "CMAKE_CXX_FLAGS:STRING:::C++ flags."
    "CMAKE_C_FLAGS:STRING:::C flags."
    "CONTINUOUS_INTEGRATION_BUILD:BOOL:OFF::Disable error redirection and timing tests"
    "UAVCAN_CMAKE_VERBOSE:BOOL:OFF::Verbose CMake configure output"
    "UAVCAN_CPP_VERSION:STRING:2011:2011 2014 2017:CPP version to force."
    )
foreach(_opt ${opts})
    # arguments are : delimited
    string(REPLACE ":" ";" _opt ${_opt})
    list(GET _opt 0 _name)
    list(GET _opt 1 _type)
    list(GET _opt 2 _default)
    list(GET _opt 3 _options)
    list(GET _opt 4 _descr)
    # options are space delimited
    string(REPLACE " " ";" _options "${_options}")
    # if a default has not already been defined, use default from table
    if(NOT DEFINED DEFAULT_${_name})
        set(DEFAULT_${_name} ${_default})
    endif()
    # option has not been set already or it is empty, set it with the default
    if(NOT DEFINED ${_name} OR ${_name} STREQUAL "")
        set(${_name} ${DEFAULT_${_name}})
    endif()
    # create a cache from the variable and force it to set
if(UAVCAN_CMAKE_VERBOSE)
    message(STATUS "${_name}\t: ${${_name}} : ${_descr}")
endif()
    set("${_name}" "${${_name}}" CACHE "${_type}" "${_descr}" FORCE)
    # if an options list is provided for the cache, set it
    if("${_type}" STREQUAL "STRING" AND NOT "${_options}" STREQUAL "")
        set_property(CACHE ${_name} PROPERTY STRINGS ${_options})
    endif()
endforeach()

#
# Set flags
#
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/uavcan/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/uavcan/dsdlc
    )

#
# Install
#
# DSDL definitions
install(DIRECTORY dsdl DESTINATION share/uavcan)

#
# Subdirectories
#
add_subdirectory(src)
# vim: set et ft=cmake fenc=utf-8 ff=unix sts=4 sw=4 ts=4 :
