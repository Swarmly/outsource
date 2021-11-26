# A very basic find g3log implementation.
# Will set G3LOG_INCLUDE_DIR and G3LOG_LIBRARY variables.
# TODO add a version check.

MESSAGE("Searching G3logger ...")

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
  SET(G3LOG_INCLUDE_DIR "/usr/include/g3log/")
  IF( IS_DIRECTORY ${G3LOG_INCLUDE_DIR})
    SET(G3LOG_LIBRARY "/usr/lib64/libg3logger.so")
    IF (EXISTS ${G3LOG_LIBRARY})
      MESSAGE("Found g3logger ${G3LOG_INCLUDE_DIR} ${G3LOG_LIBRARY}")
    ELSE()
      MESSAGE(FATAL_ERROR "${G3LOG_LIBRARY} not found")
    ENDIF()
  ELSE()
    MESSAGE(FATAL_ERROR "${G3LOG_INCLUDE_DIR} is not a directory")
  ENDIF()
ELSE()
  MESSAGE(FATAL_ERROR "OS is not Linux")
ENDIF()
