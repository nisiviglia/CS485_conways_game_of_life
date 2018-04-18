CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(Execution)

INCLUDE_DIRECTORIES("include")
INCLUDE_DIRECTORIES("${ObjLib_SOURCE_DIR}/include/all")

#-------------------------------------------------
#
# Unit Tests
#
#-------------------------------------------------
IF(BUILD_TESTS)
  ENABLE_TESTING()
  ADD_DEFINITIONS(-ggdb)
  # A simple object library used for testing execution.
  SET(TestObj_SOURCES
    "src/cell_test.cc"
    "src/frame_test.cc"
    "src/world_test.cc"
    "src/mock_cell.cc"
    "src/schema_provider.cc"
    )

  ADD_LIBRARY(TestObj OBJECT ${TestObj_SOURCES})
  SET(TestObj_TARGET_OBJECTS $<TARGET_OBJECTS:TestObj>)
  
  # SimpleCell Test.
  ADD_EXECUTABLE(SimpleCellTest
    "src/simple_cell_test.cc"
    "src/hash_map_frame.cc"
      ${TestObj_TARGET_OBJECTS}
    )
  
  TARGET_LINK_LIBRARIES(SimpleCellTest gmock_main objlib)
  ADD_TEST(SimpleCellTest SimpleCellTest)
  
  # HashMapFrame tests
  ADD_EXECUTABLE(HashMapFrameTest
    "src/hash_map_frame.cc"
    "src/hash_map_frame_test.cc"
    ${TestObj_TARGET_OBJECTS}
    )

  TARGET_LINK_LIBRARIES(HashMapFrameTest gmock_main objlib)
  ADD_TEST(HashMapFrameTest HashMapFrameTest)

  # Dispatch Tests.
  ADD_EXECUTABLE(DispatchTest
    "src/dispatch.cc"
    "src/dispatch_test.cc"
    "src/hash_map_frame.cc"
    "src/simple_world.cc"
    ${TestObj_TARGET_OBJECTS}
    )

  TARGET_LINK_LIBRARIES(DispatchTest gmock_main objlib)
  ADD_TEST(DispatchTest DispatchTest)
  
  # SimpleWorld tests
  ADD_EXECUTABLE(SimpleWorldTest
    "src/simple_world.cc"
    "src/simple_world_test.cc"
    "src/hash_map_frame.cc"
    ${TestObj_TARGET_OBJECTS}
    )

  TARGET_LINK_LIBRARIES(SimpleWorldTest gmock_main objlib)
  ADD_TEST(SimpleWorldTest SimpleWorldTest)	

  # Integration test
  ADD_EXECUTABLE(IntegrationTest
    "src/obj_lib_integration.cc"
    "src/hash_map_frame.cc"
    ${TestObj_TARGET_OBJECTS}
    )
  TARGET_LINK_LIBRARIES(IntegrationTest gmock objlib)
  ADD_TEST(IntegrationTest IntegrationTest)

ENDIF()

FIND_PACKAGE(Threads)
ADD_EXECUTABLE(ExecutionMain
  "src/main.cc"
  "src/pipe.cc"
  "src/dispatch.cc"
  "src/hash_map_frame.cc"
  "src/simple_world.cc"
  # TODO: Add other executables as main gets more features
  )
TARGET_LINK_LIBRARIES(ExecutionMain objlib ${CMAKE_THREAD_LIBS_INIT})
