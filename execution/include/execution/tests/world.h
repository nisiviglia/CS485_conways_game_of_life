#ifndef EXECUTION_TESTS_WORLD_H_
#define EXECUTION_TESTS_WORLD_H_

#include "execution/interfaces/world.h"

#include <memory>
#include <utility>

#include "gtest/gtest.h"

#include "execution/tests/mock_frame.h"

namespace execution {
// This test case provides general tests that all implementations of
// WorldInterface and WorldFactoryInterface should adhere to.
// Therefore, implementations are expected to instantiate these abstract tests.
class WorldInterfaceTest :
    public ::testing::TestWithParam<WorldFactoryInterface *> {
 protected:
  ~WorldInterfaceTest() override = default;

  static const int kId = 42;
  static constexpr char kTestName[] = "hello, world";

  std::unique_ptr<MockFrameFactory> MakeMockFrameFactory() {
    auto frame_factory = std::unique_ptr<MockFrameFactory>(new MockFrameFactory);
    frame_factory->SetCellFactoryConcrete(new MockCellFactory);
    return frame_factory;
  }

  WorldFactoryInterface *factory_ = GetParam();
};
}  // namespace execution


#endif  // EXECUTION_TESTS_WORLD_H_

