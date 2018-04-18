#ifndef EXECUTION_TESTS_FRAME_H_
#define EXECUTION_TESTS_FRAME_H_

#include "execution/interfaces/frame.h"

#include <memory>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "execution/tests/mock_cell.h"

namespace execution {
// This test case provides general tests that all implementations of
// FrameInterface and FrameFactoryInterface should adhere to.
// Therefore, implementations are expected to instantiate these abstract tests.
class FrameInterfaceTest :
    public ::testing::TestWithParam<FrameFactoryInterface *> {
 protected:
  ~FrameInterfaceTest() override = default;

  // Test frame Id.
  static const int kId;
  // Some random test points.
  static const std::vector<std::pair<int, int>> kPoints;

  // Helper for constructing MockCellFactory with default behaviors enabled.
  std::unique_ptr<MockCellFactory> MakeDefaultMockCellFactory() {
    std::unique_ptr<MockCellFactory> mock(new MockCellFactory);
    mock->EnableDefaultBehaviors();
    return std::move(mock);
  }

  // Used to construct the frame implementation being tested.
  FrameFactoryInterface *factory_ = GetParam();
};
}  // namespace execution

#endif  // EXECUTION_TESTS_FRAME_H_
