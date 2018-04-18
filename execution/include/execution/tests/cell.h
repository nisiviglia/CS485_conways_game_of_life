#ifndef EXECUTION_TESTS_CELL_H_
#define EXECUTION_TESTS_CELL_H_

#include "execution/interfaces/cell.h"

#include <memory>
#include <utility>

#include "gtest/gtest.h"

namespace execution {
// This test case provides general tests that all implementations of
// CellInterface and CellFactoryInterface should adhere to.
// Therefore, implementations are expected to instantiate these abstract tests.
class CellInterfaceTest :
    public ::testing::TestWithParam<CellFactoryInterface *> {
protected:
  ~CellInterfaceTest() override = default; 

  // Helpers for making cells where the coordinate is insignificant.
  std::unique_ptr<CellInterface> OriginCellXY(CellState state,
                                              FrameInterface *parent) {
    return factory_->MakeCell(0, 0, state, parent);
  }
  std::unique_ptr<CellInterface> OriginCellPt(CellState state,
                                              FrameInterface *parent) {
    return factory_->MakeCell(std::make_pair(0, 0), state, parent);
  }

  CellFactoryInterface *factory_ = GetParam();
};
}  // namespace execution

#endif  // EXECUTION_TESTS_CELL_H_
