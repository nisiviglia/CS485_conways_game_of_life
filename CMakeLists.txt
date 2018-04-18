CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(WebMind)

#-------------------------------------------------
#
# Gloal Settings
#
#-------------------------------------------------
SET(CMAKE_CXX_STANDARD 11)
SET(BUILD_TESTS OFF CACHE BOOL "Build unit tests for each sub-project")
ADD_DEFINITIONS(
# Adds a cpp definition with the file path of the root dir.
# Useful for specifying relative file paths.
  "-DREPO_ROOT_DIR=\"${CMAKE_SOURCE_DIR}/\""
# Tells RapidJSON to integrate with std::string.
  "-DRAPIDJSON_HAS_STDSTRING=1"
# Tells RapidJSON to verbosely diagnose schema validations.
# "-DRAPIDJSON_SCHEMA_VERBOSE=1"
  )

#-------------------------------------------------
#
# GoogleTest
#
#-------------------------------------------------
IF(BUILD_TESTS)
  MESSAGE("--------------------------------------------------")
  MESSAGE("")
  MESSAGE("BUILD_TESTS HAS BEEN ENABLED")
  MESSAGE("")
  MESSAGE("--------------------------------------------------")

  SET(GoogleTest_SOURCE_DIR "${CMAKE_BINARY_DIR}/googletest-src")
  SET(GoogleTest_BINARY_DIR "${CMAKE_BINARY_DIR}/googletest-build")

  CONFIGURE_FILE("CMakeLists.txt.in" "googletest/CMakeLists.txt")
  EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest")
  EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest")

  SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  INCLUDE_DIRECTORIES(
    "${GoogleTest_SOURCE_DIR}/googletest/include"
    "${GoogleTest_SOURCE_DIR}/googlemock/include"
    )

  ADD_SUBDIRECTORY("${GoogleTest_SOURCE_DIR}" "${GoogleTest_BINARY_DIR}")
ENDIF()

#-------------------------------------------------
#
# Sub Projects
#
#-------------------------------------------------
ADD_SUBDIRECTORY("objlib" "${WebMind_BINARY_DIR}/objlib")
ADD_SUBDIRECTORY("execution" "${WebMind_BINARY_DIR}/execution")
ADD_SUBDIRECTORY("gui" "${WebMind_BINARY_DIR}/gui")

ADD_SUBDIRECTORY("scripting_language" "${WebMind_BINARY_DIR}/scripting_language")
#ADD_SUBDIRECTORY("exec-obj-interface" "${WebMind_BINARY_DIR}/exec-obj-interface")


#add_dependencies(TestObj objlib)
add_dependencies(honors_script objlib)
