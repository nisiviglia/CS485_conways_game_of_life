#ifndef EXECUTION_TESTS_MOCK_CELL_H_
#define EXECUTION_TESTS_MOCK_CELL_H_

#include "execution/interfaces/cell.h"

#include "gmock/gmock.h"

#include "execution/impl/simple_cell.h"

namespace execution {
// This is a mocked out CellInterface. Useful for unit testing.
// Defaults to a SimpleCell implementation so that the mocked methods return
// sensible output if no expectation is made.
class MockCell : public CellInterface {
public:
  // Defers calls to a SimpleCell implementation of the CellInterface.
  // These parameters are needed to initialize that instance.
  MockCell(int x, int y, CellState state, FrameInterface *parent)
      : impl_(x, y, state, parent) {
    DefaultToSimpleCell();
  }
  MockCell(std::pair<int, int> pt, CellState state, FrameInterface *parent)
      : impl_(pt, state, parent) {
    DefaultToSimpleCell();
  }
  // Virtual destructor
  ~MockCell() override = default;

  // Mocked methods.
  MOCK_CONST_METHOD0(GetPositionX, int());
  MOCK_CONST_METHOD0(GetPositionY, int());
  MOCK_CONST_METHOD0(GetPosition, std::pair<int, int>());

  MOCK_CONST_METHOD0(GetState, CellState());
  MOCK_CONST_METHOD0(IsAlive, bool());

  MOCK_CONST_METHOD0(GetParent, FrameInterface *());
  
  // NOTE: If you add more mocked methods here, make sure to add the
  //       corresponding ON_CALLs to DefaultToSimpleCell.

private:
  // Set all mock methods to defer to impl_ by default.
  void DefaultToSimpleCell();

  SimpleCell impl_;
};

// This is a mocked out CellFactoryInterface. Useful for unit testing.
class MockCellFactory : public CellFactoryInterface {
public:
  // Virtual destructor
  ~MockCellFactory() override = default;

  // Provides default implementations of MakeCell that return new MockCells,
  // as well as handling the parent pointer correctly.
  void EnableDefaultBehaviors();

  // Mocked methods.
  MOCK_METHOD4(MakeCell, std::unique_ptr<CellInterface>(
        int x, int y, CellState state, FrameInterface *parent));
  MOCK_METHOD3(MakeCell, std::unique_ptr<CellInterface>(
        std::pair<int, int> pt, CellState state, FrameInterface *parent));
};
}  // namespace execution

#endif  // EXECUTION_TESTS_CELL_MOCK_H_
