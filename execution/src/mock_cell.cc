#include "execution/tests/mock_cell.h"

using ::testing::ByRef;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::_;

namespace execution {
namespace {
// Helper function for forwarding MakeCell calls to new MockCell.
std::unique_ptr<CellInterface> MakeMockCell(int x, int y, CellState state,
                                            FrameInterface *parent) {
  return std::unique_ptr<CellInterface>(new MockCell(x, y, state, parent));
}
}  // namespace anonymous

// Set all methods to forward to impl_ by default.
void MockCell::DefaultToSimpleCell() {
  ON_CALL(*this, GetPositionX())
    .WillByDefault(Invoke(&impl_, &SimpleCell::GetPositionX));
  ON_CALL(*this, GetPositionY())
    .WillByDefault(Invoke(&impl_, &SimpleCell::GetPositionY));
  ON_CALL(*this, GetPosition())
    .WillByDefault(Invoke(&impl_, &SimpleCell::GetPosition));
  ON_CALL(*this, GetState())
    .WillByDefault(Invoke(&impl_, &SimpleCell::GetState));
  ON_CALL(*this, IsAlive())
    .WillByDefault(Invoke(&impl_, &SimpleCell::IsAlive));
  ON_CALL(*this, GetParent())
    .WillByDefault(Invoke(&impl_, &SimpleCell::GetParent));
}

// Set MakeCell mocks to return new MockCells.
void MockCellFactory::EnableDefaultBehaviors() {
  // Make Mock Cells.
  ON_CALL(*this, MakeCell(_, _, _, _))
    .WillByDefault(Invoke([this] (int x, int y, CellState state,
                                  FrameInterface *parent) {
          return MakeMockCell(x, y, state, parent);
        }));
  ON_CALL(*this, MakeCell(_, _, _))
    .WillByDefault(Invoke([this] (std::pair<int, int> pt, CellState state,
                                  FrameInterface *parent) {
          return MakeMockCell(pt.first, pt.second, state, parent);
        }));
}
}  // namespace execution
