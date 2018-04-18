CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(scripting_language)

INCLUDE_DIRECTORIES("${scripting_language_SOURCE_DIR}/include" "${ObjLib_SOURCE_DIR}/include/all")

IF(WIN32)
  INCLUDE_DIRECTORIES("${scripting_language_SOURCE_DIR}/include" "${ObjLib_SOURCE_DIR}/include/win")
ENDIF()

file(GLOB scripLang_SOURCES
# "${scripting_language_SOURCE_DIR}/include/*.h"
  "${scripting_language_SOURCE_DIR}/src/*.cpp"
# "include/"
# "../objlib/include/all/ObjDispatcher.hpp"
)

SET(CMAKE_CXX_FLAGS "-g")

ADD_EXECUTABLE(honors_script ${scripLang_SOURCES})
TARGET_LINK_LIBRARIES(honors_script objlib)

#location for honors_script executable
ADD_DEFINITIONS(-DHONORS_SCRIPT="${scripting_language_BINARY_DIR}/honors_script")
ADD_DEFINITIONS(-DTEST_DIR="${scripting_language_SOURCE_DIR}/test/")

#----------------------------------------------------
#
# Unit Tests
#
#----------------------------------------------------
IF(BUILD_TESTS)
  ENABLE_TESTING()

  # Enter scripting_language tests here
  ADD_EXECUTABLE(ScriptTest "test/ScriptTest.cpp")
  TARGET_LINK_LIBRARIES(ScriptTest objlib gtest_main)
  ADD_TEST(ScriptTest ScriptTest)
  # ADD_TEST(WorldTest WorldTest)

ENDIF()