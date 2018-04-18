#include "execution/tests/cell.h"

// These are the tests for the CellInterfaceTest fixture.
namespace execution {
namespace {
TEST_P(CellInterfaceTest, CellsInheritParentFromFactoryXY) {
  auto cell_xy = OriginCellXY(CellState::kDead, nullptr);

  EXPECT_EQ(cell_xy->GetParent(), nullptr);
}

TEST_P(CellInterfaceTest, CellsInheritParentFromFactoryPt) {
  auto cell_pt = OriginCellPt(CellState::kDead, nullptr);

  EXPECT_EQ(cell_pt->GetParent(), nullptr);
}

TEST_P(CellInterfaceTest, CellsRetainCoordinatePairXY) {
  const int kX = -5, kY = 17;

  auto cell_xy = factory_->MakeCell(kX, kY, CellState::kDead, nullptr);

  EXPECT_EQ(cell_xy->GetPositionX(), kX);
  EXPECT_EQ(cell_xy->GetPositionY(), kY);
  EXPECT_EQ(cell_xy->GetPosition(), std::make_pair(kX, kY));
}

TEST_P(CellInterfaceTest, CellsRetainCoordinatePairPt) {
  const auto kPt = std::make_pair(42, -213);

  auto cell_pt = factory_->MakeCell(kPt, CellState::kDead, nullptr);

  EXPECT_EQ(cell_pt->GetPositionX(), kPt.first);
  EXPECT_EQ(cell_pt->GetPositionY(), kPt.second);
  EXPECT_EQ(cell_pt->GetPosition(), kPt);
}

TEST_P(CellInterfaceTest, DeadCellsAreDeadXY) {
  auto cell_xy = OriginCellXY(CellState::kDead, nullptr);

  EXPECT_EQ(cell_xy->GetState(), CellState::kDead);
  EXPECT_FALSE(cell_xy->IsAlive());
}

TEST_P(CellInterfaceTest, DeadCellsAreDeadPt) {
  auto cell_pt = OriginCellPt(CellState::kDead, nullptr);

  EXPECT_EQ(cell_pt->GetState(), CellState::kDead);
  EXPECT_FALSE(cell_pt->IsAlive());
}

// NOTE: The following tests are likely to change if the CellState type is
// replaced by a more complex type that allows for more than one alive state.
TEST_P(CellInterfaceTest, AliveCellsAreAliveXY) {
  auto cell_xy = OriginCellXY(CellState::kAlive, nullptr);

  EXPECT_EQ(cell_xy->GetState(), CellState::kAlive);
  EXPECT_TRUE(cell_xy->IsAlive());
}

TEST_P(CellInterfaceTest, AliveCellsAreAlivePt) {
  auto cell_pt = OriginCellPt(CellState::kAlive, nullptr);

  EXPECT_EQ(cell_pt->GetState(), CellState::kAlive);
  EXPECT_TRUE(cell_pt->IsAlive());
}
}  // anonymous namespace
}  // namespace execution
