project(QtAVWidgets LANGUAGES CXX)
include(gtu-versions)
set(EXTRA_DEFS -DBUILD_QTAVWIDGETS_LIB)
list(APPEND EXTRA_DEFS -DQTAV_HAVE_GL=1)

set(HAVE_QT5OPENGL 1)
if(BUILD_QT5OPENGL)
  find_package(Qt5OpenGL)
  if(Qt5OpenGL_FOUND)
    set(HAVE_QT5OPENGL 1)
  endif()
endif()

set(SDK_HEADERS
        QtAVWidgets/global.h
        QtAVWidgets/GraphicsItemRenderer.h
        QtAVWidgets/OpenGLWidgetRenderer.h
        QtAVWidgets/QtAVWidgets
        QtAVWidgets/QtAVWidgets.h
        QtAVWidgets/version.h
        QtAVWidgets/VideoPreviewWidget.h
        QtAVWidgets/WidgetRenderer.h
        )
file(GLOB SDK_PRIVATE_HEADERS QtAVWidgets/*.h)

set(SOURCES
        global.cpp
        GraphicsItemRenderer.cpp
        OpenGLWidgetRenderer.cpp
        VideoPreviewWidget.cpp
        WidgetRenderer.cpp
        )
if(HAVE_QT5OPENGL)
  list(APPEND SDK_HEADERS QtAVWidgets/GLWidgetRenderer2.h)
  list(APPEND SOURCES GLWidgetRenderer2.cpp)
endif()

# TODO: Qt5Gui_OPENGL_IMPLEMENTATION?
if(Qt5Gui_VERSION VERSION_LESS 5.4.0)
  list(APPEND SDK_HEADERS QtAVWidgets/QOpenGLWidget.h)
  list(APPEND SOURCES QOpenGLWidget.cpp)
endif()

set(HEADERS ${SDK_HEADERS})
if(WIN32)
  set(RC_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.rc)
  configure_file(${CMAKE_SOURCE_DIR}/cmake/QtAV.rc.in ${RC_FILE})
endif()
# add HEADERS for moc
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${RESOURCES_SOURCES} ${HEADERS} ${RC_FILE})
target_link_libraries(${PROJECT_NAME}
        LINK_PRIVATE ${EXTRA_LIBS}
        LINK_PUBLIC QtAV Qt5::Widgets
        )
if(HAVE_QT5OPENGL)
  target_link_libraries(${PROJECT_NAME} LINK_PUBLIC Qt5::OpenGL)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_RPATH ON
        FRAMEWORK ON
        #VERSION ${PROJECT_VERSION}
        SOVERSION ${SO_VERSION}
        OUTPUT_NAME ${PROJECT_NAME}
        CLEAN_DIRECT_OUTPUT 1
        #LINK_SEARCH_START_STATIC 1
        #LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        )


#http://stackoverflow.com/questions/19866424/cmake-and-qt5-automoc-error
#http://doc.qt.io/qt-5/cmake-manual.html
set(CMAKE_INCLUDE_CURRENT_DIR ON) # for .moc
target_include_directories(${PROJECT_NAME}
        PUBLIC $<INSTALL_INTERFACE:include>
        PUBLIC $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}>
        PRIVATE ${EXTRA_INCLUDE}
        )
target_compile_definitions(${PROJECT_NAME} PRIVATE ${EXTRA_DEFS})

set(PROJECT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${VERSION_STRING} SOVERSION ${VERSION_STRING})
set(COMPONENT_NAME ${PROJECT_NAME})
set(COMPONENT_NAME_DEV ${PROJECT_NAME}-dev)

set(COMPONENT_CONFIG_NAME ${COMPONENT_NAME}Config)

# install libs
install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${COMPONENT_CONFIG_NAME}
        DESTINATION ${GTU_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${GTU_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${GTU_INSTALL_LIBDIR}
)
# install dev headers
install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${GTU_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.h*"
)

# This makes the project importable from the build directory
export(TARGETS ${PROJECT_NAME} FILE cmake/${COMPONENT_NAME_DEV}Config.cmake)
#install(FILES
#  ${SDK_HEADERS}
#  DESTINATION ${QTAV_INSTALL_HEADERS}/${PROJECT_NAME}
#)
#install(FILES
#  ${SDK_PRIVATE_HEADERS}
#  DESTINATION ${QTAV_INSTALL_HEADERS}/${PROJECT_NAME}/private
#)
#install(TARGETS ${PROJECT_NAME}
#  EXPORT ${PROJECT_NAME}-targets
#  #PUBLIC_HEADER DESTINATION ${QTAV_INSTALL_HEADERS}/${PROJECT_NAME}
#  #PRIVATE_HEADER DESTINATION ${QTAV_INSTALL_HEADERS}/${PROJECT_NAME}/private
#  RUNTIME DESTINATION ${QTAV_INSTALL_BINS}
#  LIBRARY DESTINATION ${QTAV_INSTALL_LIBS}
#  ARCHIVE DESTINATION ${QTAV_INSTALL_LIBS}
#  FRAMEWORK DESTINATION ${QTAV_INSTALL_LIBS}
#)
#install(EXPORT ${PROJECT_NAME}-targets
#  DESTINATION ${QTAV_INSTALL_LIBS}/cmake/${PROJECT_NAME}
#  FILE ${PROJECT_NAME}-config.cmake
#)
