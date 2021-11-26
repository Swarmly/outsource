project(hermes-gui LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libQtAV)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libQtAVWidgets)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libQtAVWidgets/QtAVWidgets)

find_package(FFMPEG COMPONENTS avformat avutil avdevice avcodec swscale swresample REQUIRED)
#find_package(OpenGL REQUIRED)
add_definitions(-DGTU_CORE_LIBRARY)
add_definitions(-DHERMES_GUI_LIBRARY)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

GTU_DEFINE_LIB()

set(target_gui_dependencies Qt5::Widgets QtAV QtAVWidgets)

target_link_libraries(${PROJECT_NAME} PRIVATE ${target_gui_dependencies} ${FFMPEG_LIBRARIES})

#target_link_libraries(${PROJECT_NAME} PRIVATE QtAV)
#gtu_get_non_test_library_list(non_test_libraries)
#add_dependencies(${PROJECT_NAME} ${non_test_libraries})
