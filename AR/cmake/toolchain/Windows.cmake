set(TARGET_OS WINDOWS)

if (MINGW)
    if(NOT CMAKE_GENERATOR STREQUAL "Ninja")
        message(WARNING "Generator “${CMAKE_GENERATOR}” is unsupported!\nTry Ninja if you encounter problems.")
    endif()
endif()

if (ToolchainTarget STREQUAL ${ToolchainHost})
    # Try to automatically detect MSYS2 and MinGW paths
    if (NOT DEFINED MSYS2_PATH)
        # Set MingW bin dir, if MingW present
        STRING(REGEX REPLACE "([^ ]+)[/\\].*" "\\1" MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}")
        STRING(REGEX REPLACE "\\\\" "/" MINGW_BIN_DIR "${MINGW_BIN_DIR}") # Replace back slashes to slashes
        if (NOT ${MINGW_BIN_DIR} STREQUAL "")
            get_filename_component(MINGW_PATH ${MINGW_BIN_DIR} DIRECTORY)
        endif()
        if (NOT ${MINGW_PATH} STREQUAL "")
            get_filename_component(MSYS2_PATH ${MINGW_PATH} DIRECTORY)
        endif()
    endif()
    if (NOT DEFINED MSYS2_PATH)
        # Get from env variable MSYS2_PATH
        if (DEFINED ENV{MSYS2_PATH})
            set(MSYS2_PATH "${ENV{MSYS2_PATH}")
            set(MINGW_PATH "${MSYS2_PATH}/mingw64")
            set(MINGW_BIN_DIR "${MINGW_PATH}/bin")
        endif()
    endif()
    # Seems msys2 is failed to detect automatically, try to ask the developer and show him a dialog
    if (NOT DEFINED MSYS2_PATH)
        # Directory chooser dialog.
        set(MSYS2_PATH "" CACHE PATH "Path to the msys2 installation directory")
        if(NOT MSYS2_PATH)
            message(FATAL_ERROR "MSYS2_PATH not set!")
        endif()
        set(MINGW_PATH "${MSYS2_PATH}/mingw64")
        set(MINGW_BIN_DIR "${MINGW_PATH}/bin")
    endif()

    macro(get_WIN32_WINNT version)
        if(CMAKE_SYSTEM_VERSION)
            set(ver ${CMAKE_SYSTEM_VERSION})
            string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
            string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
            # Check for Windows 10, b/c we'll need to convert to hex 'A'.
            if("${verMajor}" MATCHES "10")
                set(verMajor "A")
                string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
            endif()
            # Remove all remaining '.' characters.
            string(REPLACE "." "" ver ${ver})
            # Prepend each digit with a zero.
            string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
            set(${version} "0x${ver}")
        endif()
    endmacro()

    get_WIN32_WINNT(ver)
    add_definitions(-D_WIN32_WINNT=${ver})
else()
    # TODO: Implement for crosscompiling
    message(FATAL_ERROR "Crosscompiling is not supported yet!")
endif()

# Find bash
find_program(BASH_EXECUTABLE
        NAMES bash
        HINTS ${MINGW_PATH} ${MSYS2_PATH} "${MSYS2_PATH}/usr/bin"
        REQUIRED
)

# Find env
find_program(ENV_EXECUTABLE
        NAMES env
        HINTS ${MINGW_PATH} ${MSYS2_PATH} "${MSYS2_PATH}/usr/bin"
        REQUIRED
)

# TODO: add message if failed to find bash and env