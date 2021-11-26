if(DESKTOP_BUILD)

include(GNUInstallDirs)

if(GIT_FOUND)
    set(APPDATA_RELEASE_VERSION ${PROJECT_VERSION}-${GIT_COMMIT_HASH})
    execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=%cd --date=short
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE APPDATA_RELEASE_DATE
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(APPDATA_RELEASE_VERSION ${PROJECT_VERSION})
    file(TIMESTAMP "${CMAKE_SOURCE_DIR}/CMakeLists.txt" APPDATA_RELEASE_DATE "%Y-%m-%d")
endif()

function(gtu_set_icons APP_ID)
        # TODO: either generate icons with good quality for different resolutions or use scalable icon
        foreach(size 128)
        install(
                FILES ../icons/${size}x${size}/${APP_ID}.png
                DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/${size}x${size}/apps
        )
        # TODO: add the separate icon for *.ts and *json files
        install(
                FILES ../icons/${size}x${size}/${APP_ID}.png
                DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/${size}x${size}/mimetypes
        )
        endforeach()
endfunction()

function(gtu_linux_configure_and_install_desktop_file APP_ID)
        configure_file(
                ${APP_ID}.desktop.in
                ${APP_ID}.desktop
        )
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${APP_ID}.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)

        # TODO: either generate icons with good quality for different resolutions or use scalable icon
        gtu_set_icons(${APP_ID})
endfunction()

function(gtu_linux_install_shell_launching_data APP_ID)       
        # configure install .appdata.xml
        configure_file(
                ${APP_ID}.appdata.xml.in
                ${APP_ID}.appdata.xml
        )
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${APP_ID}.appdata.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo)

        # configure and install .desktop file
        gtu_linux_configure_and_install_desktop_file(${APP_ID})

        # extensions
        install(FILES ${APP_ID}.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/mime/packages)
endfunction()

add_subdirectory(hermes)
add_subdirectory(hmtool)
add_subdirectory(misc-tools)

set(_HERMES_COMPONENT_DEPENDENCIES "")

install(
        FILES ${PROJECT_SOURCE_DIR}/GTU/sitl/bin/arduplane ${PROJECT_SOURCE_DIR}/GTU/sitl/bin/arducopter
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        DESTINATION /software/gtu/sitl/bin
        )

    install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/GTU/sitl/default_params
        DESTINATION /software/gtu/sitl
        FILES_MATCHING PATTERN "*.*"
        )


    install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/GTU/sitl/eeprom
        DESTINATION /software/gtu/sitl
        FILES_MATCHING PATTERN "*.*"
        )

    set(_HERMES_COMPONENT_DEPENDENCIES mc minicom vlc exiv2)
    if (WITH_3D)
        list(APPEND _HERMES_COMPONENT_DEPENDENCIES libqt53dextras5 libqt53drender5)
    endif()
    ###QGroundControl#############################
    ###QGroundControl#############################
    ###QGroundControl#############################
#    install(
#        FILES ${PROJECT_SOURCE_DIR}/GTU/QGroundControl/QGroundControl
#        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
#        DESTINATION /software/QGroundControl/
#        )

#    install(
#        FILES ${PROJECT_SOURCE_DIR}/GTU/QGroundControl/qgroundcontrol-start.sh
#        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
#        DESTINATION /software/QGroundControl/
#        )

#    install(
#        DIRECTORY ${PROJECT_SOURCE_DIR}/GTU/QGroundControl
#        DESTINATION /software
#        FILES_MATCHING PATTERN "*.desktop"
#        )
#    install(
#        DIRECTORY ${PROJECT_SOURCE_DIR}/GTU/QGroundControl
#        DESTINATION /software
#        FILES_MATCHING PATTERN "*.png"
#        )

    #install(
    #    DIRECTORY ${PROJECT_SOURCE_DIR}/GTU/QGroundControl
    #    DESTINATION /software
    #    FILES_MATCHING PATTERN "*.qml"
    #    )
    #install(
    #    DIRECTORY ${PROJECT_SOURCE_DIR}/GTU/QGroundControl
    #    DESTINATION /software
    #    FILES_MATCHING PATTERN "*.so"
    #    )


    #install(
    #    DIRECTORY ${PROJECT_SOURCE_DIR}/GTU/QGroundControl
    #    DESTINATION /software
    #    FILES_MATCHING PATTERN "*.*"
    #    )

    #install(
    #    DIRECTORY icons ${PROJECT_SOURCE_DIR}/GTU/QGroundControl/Qt/qml
    #    DESTINATION /software/QGroundControl/
    #    FILES_MATCHING PATTERN "*.*"
    #    )


    ###QGroundControl#############################
    ###QGroundControl#############################
    ###QGroundControl#############################
    set(EXTRA_NAME "_desktop")

    message (STATUS "Drivers BUILD DISABLED")
else(DESKTOP_BUILD)
    message (STATUS "Desktop BUILD DISABLED")

    install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/GTU/drivers/hermes
        DESTINATION /lib/modules/${CMAKE_SYSTEM_PROCESSOR}/kernel/
        FILES_MATCHING PATTERN "*.ko"
        )
    set(_HERMES_COMPONENT_DEPENDENCIES mc minicom)
endif(DESKTOP_BUILD)

install(
    FILES ${PROJECT_SOURCE_DIR}/GTU/scripts/sitl.sh ${PROJECT_SOURCE_DIR}/GTU/scripts/flash.sh ${PROJECT_SOURCE_DIR}/GTU/scripts/reset.sh ${PROJECT_SOURCE_DIR}/GTU/scripts/start.sh  ${PROJECT_SOURCE_DIR}/GTU/scripts/stop.sh ${PROJECT_SOURCE_DIR}/GTU/scripts/close.sh
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    DESTINATION /software/gtu/scripts
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/GTU/key/
    DESTINATION /software/gtu/key/
    FILES_MATCHING PATTERN "*.*"
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/ext
    DESTINATION /software/gtu
    FILES_MATCHING PATTERN "*.*"
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/GTU/etc/modprobe.d
    DESTINATION /etc
    FILES_MATCHING PATTERN "*.*"
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/GTU/etc/system
    DESTINATION /etc/systemd
    FILES_MATCHING PATTERN "*.*"
)

# Create DEB packages
set(CPACK_SYSTEM_NAME ${CMAKE_LIBRARY_ARCHITECTURE}${EXTRA_NAME}_${GIT_COMMIT_HASH})
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_NAME "HERMES")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "TODO:")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://www.easy-fpv.com/")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "HERMES")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_ARCHIVE_TYPE gnutar)
set(CPACK_DEBIAN_PACKAGE_SECTION science)
set(CPACK_DEBIAN_PACKAGE_PRIORITY optional)
set(CPACK_DEBIAN_COMPRESSION_TYPE gzip)
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

set(CPACK_DEB_COMPONENT_INSTALL ON) # enable to generate mulitple packages
#set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
#set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON) # enable to add inner dependencies between components

# Configs package
set(_HERMES_CONFIGS_PACKAGE_NAME "${PROJECT_NAME}-configs")
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/GTU/cfg/
    COMPONENT ${_HERMES_CONFIGS_PACKAGE_NAME}
    DESTINATION /software/gtu/cfg/
    FILES_MATCHING PATTERN "*.*"
)

# Add test_data to dev package
install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/test_data/
        COMPONENT ${GTU_INSTALL_DEV_COMPONENT_NAME}
        DESTINATION /software/gtu/test_data/
        FILES_MATCHING PATTERN "*.*"
)

include(CPackComponent)

function(_gtu_define_deb_component COMPONENT_NAME)
        # define var for component name in uppercase
        string(TOUPPER "${COMPONENT_NAME}" _COMPONENT_NAME_UC)
        set(_${_COMPONENT_NAME_UC}_COMPONENT_NAME_UC ${_COMPONENT_NAME_UC} PARENT_SCOPE)

        # override file name and package name
        set(CPACK_DEBIAN_${_COMPONENT_NAME_UC}_FILE_NAME ${COMPONENT_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}.deb PARENT_SCOPE)
        set(CPACK_DEBIAN_${_COMPONENT_NAME_UC}_PACKAGE_NAME ${COMPONENT_NAME} PARENT_SCOPE)

        # add new component if it's not default one
        if (NOT COMPONENT_NAME STREQUAL ${CMAKE_INSTALL_DEFAULT_COMPONENT_NAME})
                cpack_add_component_group(${COMPONENT_NAME})
                cpack_add_component(
                        ${COMPONENT_NAME}
                        GROUP ${COMPONENT_NAME}
                )
        endif()
endfunction()

# define hermes package
_gtu_define_deb_component(${CMAKE_INSTALL_DEFAULT_COMPONENT_NAME})
set(CPACK_DEBIAN_${_HERMES_COMPONENT_NAME_UC}_PACKAGE_DEPENDS ${_HERMES_COMPONENT_DEPENDENCIES})

# define hermes-configs package
_gtu_define_deb_component(${_HERMES_CONFIGS_PACKAGE_NAME})

# define hermes-dev package
_gtu_define_deb_component(${GTU_INSTALL_DEV_COMPONENT_NAME})
set(CPACK_DEBIAN_${_HERMES-DEV_COMPONENT_NAME_UC}_PACKAGE_SHLIBDEPS OFF)

# define new deb package here

include(CPack)
