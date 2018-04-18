CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(ObjLib)

INCLUDE_DIRECTORIES("${ObjLib_SOURCE_DIR}/include/all")


IF(WIN32)
  INCLUDE_DIRECTORIES("${ObjLib_SOURCE_DIR}/include/win")
ENDIF()

FILE(GLOB_RECURSE ObjLib_SOURCES
  "${ObjLib_SOURCE_DIR}/src/*.cpp"
  "${ObjLib_SOURCE_DIR}/src/*.c"
  )

ADD_LIBRARY(objlib SHARED ${ObjLib_SOURCES})

#-------------------------------------------------
#
# Unit Tests
#
#-------------------------------------------------
IF(BUILD_TESTS)
  ENABLE_TESTING()

  ADD_EXECUTABLE(WorldTest "test/WorldTest.cpp")
  TARGET_LINK_LIBRARIES(WorldTest objlib gtest_main)
  ADD_TEST(WorldTest WorldTest)

  ADD_EXECUTABLE(FrameTest "test/FrameTest.cpp")
  TARGET_LINK_LIBRARIES(FrameTest objlib gtest_main)
  ADD_TEST(FrameTest FrameTest)

  ADD_EXECUTABLE(ObjDispatcherTest "test/ObjDispatcherTest.cpp")
  TARGET_LINK_LIBRARIES(ObjDispatcherTest objlib gtest_main)
  ADD_TEST(ObjDispatcherTest ObjDispatcherTest)
ENDIF()

