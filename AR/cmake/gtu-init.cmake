list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_HOST_EXECUTABLE_SUFFIX "")
if(CMAKE_HOST_WIN32)
    set(CMAKE_HOST_EXECUTABLE_SUFFIX ".exe")
endif()

# Toolchain host detection
# It's not possible to use CMAKE_HOST_SYSTEM_NAME at this point
# because that's only assigned when project() is called which comes later.
if(CMAKE_HOST_APPLE)
    set(ToolchainHost "Darwin")
elseif(CMAKE_HOST_WIN32)
    set(ToolchainHost "Windows")
elseif(CMAKE_HOST_UNIX)
    set(LINUX TRUE)
    set(ToolchainHost "Linux")
else()
    message(FATAL_ERROR "Unknown host operating system")
endif()

if(CMAKE_SYSTEM_NAME)
    set(ToolchainTarget "${CMAKE_SYSTEM_NAME}")
else()
    set(ToolchainTarget "${ToolchainHost}")
endif()

# Include the appropriate toolchain file
# Currently in only Windows toolchain is available
if (WIN32)
    include("toolchain/${ToolchainTarget}")
endif()

# Find Git. GIT_FOUND and GIT_EXECUTABLE are set up.
find_package(Git)

if (NOT GIT_FOUND)
    message(FATAL_ERROR "Git was not find! Make sure you installed git.")
endif()