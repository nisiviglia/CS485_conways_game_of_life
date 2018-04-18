#include "execution/tests/frame.h"

#include "gmock/gmock.h"

using ::testing::Assign;
using ::testing::AtMost;
using ::testing::ByMove;
using ::testing::DoAll;
using ::testing::DoDefault;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::UnorderedElementsAreArray;
using ::testing::_;


namespace execution {

// Static members.
const int FrameInterfaceTest::kId = 42; 
const std::vector<std::pair<int, int>> FrameInterfaceTest::kPoints = {
  {42, 12}, {-5, 134}, {0, 0}, {-1, 42}, {27, -9},
};

namespace {
TEST_P(FrameInterfaceTest, BlankFrameIsBlank) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  // Allow frame to make dead cells (but not required).
  EXPECT_CALL(*mock_cell_factory, MakeCell(_, _, CellState::kDead, _))
    .WillRepeatedly(DoDefault());
  EXPECT_CALL(*mock_cell_factory, MakeCell(_, CellState::kDead, _))
    .WillRepeatedly(DoDefault());
  // Disallow the creation of any other type of cell.
  EXPECT_CALL(*mock_cell_factory, MakeCell(_, _, _, _)).Times(0);
  EXPECT_CALL(*mock_cell_factory, MakeCell(_, _, _)).Times(0);

  // Construct a blank frame to test if disallowed calls arise.
  factory_->SetCellFactory(mock_cell_factory.get());
  auto blank = factory_->MakeFrame(kId);
}

TEST_P(FrameInterfaceTest, QueryingBlankFrameReturnsDeadCellsPt) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  factory_->SetCellFactory(mock_cell_factory.get());
  auto blank = factory_->MakeFrame(kId);

  for (const auto &pt : kPoints) {
    EXPECT_EQ(blank->QueryCell(pt), CellState::kDead) << "Point (" << pt.first
                                                      << ", " << pt.second
                                                      << ") was alive";
  }
}

TEST_P(FrameInterfaceTest, QueryingBlankFrameReturnsDeadCellsXY) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  factory_->SetCellFactory(mock_cell_factory.get());
  auto blank = factory_->MakeFrame(kId);

  for (const auto &pt : kPoints) {
    EXPECT_EQ(blank->QueryCell(pt.first, pt.second), CellState::kDead)
      << "Point (" << pt.first << ", " << pt.second << ") was alive";
  }
}

TEST_P(FrameInterfaceTest, ToggleCellResultsInAliveQueryResultPt) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  factory_->SetCellFactory(mock_cell_factory.get());
  auto blank = factory_->MakeFrame(kId);
  
  // Toggle all test points, and query them.
  for (const auto &pt : kPoints) {
    blank->ToggleCell(pt);
    EXPECT_NE(blank->QueryCell(pt), CellState::kDead) << "Point (" << pt.first
                                                      << ", " << pt.second
                                                      << ") was still dead";
  }
}

TEST_P(FrameInterfaceTest, ToggleCellResultsInAliveQueryResultXY) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  factory_->SetCellFactory(mock_cell_factory.get());
  auto blank = factory_->MakeFrame(kId);
  
  // Toggle all test points, and query them.
  for (const auto &pt : kPoints) {
    blank->ToggleCell(pt.first, pt.second);
    EXPECT_NE(blank->QueryCell(pt.first, pt.second), CellState::kDead)
      << "Point (" << pt.first << ", " << pt.second << ") was still dead";
  }
}

TEST_P(FrameInterfaceTest, ToggleCellCreatesCellWithCorrectParent) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  factory_->SetCellFactory(mock_cell_factory.get());
  auto blank = factory_->MakeFrame(kId);

  // Not sure which one will be called, so record which one is.
  bool pt_called = false, xy_called = false;
  EXPECT_CALL(*mock_cell_factory, MakeCell(_, _, blank.get()))
    .Times(AtMost(1))
    .WillRepeatedly(DoAll(Assign(&pt_called, true),
                                 Return(ByMove(std::unique_ptr<MockCell>()))));
  EXPECT_CALL(*mock_cell_factory, MakeCell(_, _, _, blank.get()))
    .Times(AtMost(1))
    .WillRepeatedly(DoAll(Assign(&xy_called, true),
                                 Return(ByMove(std::unique_ptr<MockCell>()))));

  blank->ToggleCell(kPoints[0]);

  EXPECT_NE(pt_called, xy_called) << "Exactly one should be true";
}

TEST_P(FrameInterfaceTest, DoubleToggleCellResultsInDeadQueryResultPt) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  factory_->SetCellFactory(mock_cell_factory.get());
  auto blank = factory_->MakeFrame(kId);

  // Toggle all test points twice, and query them.
  for (const auto &pt : kPoints) {
    blank->ToggleCell(pt);
    blank->ToggleCell(pt);
    EXPECT_EQ(blank->QueryCell(pt), CellState::kDead) << "Point (" << pt.first
                                                      << ", " << pt.second
                                                      << ") was still alive";
  }
}

TEST_P(FrameInterfaceTest, DoubleToggleCellResultsInDeadQueryResultXY) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  factory_->SetCellFactory(mock_cell_factory.get());
  auto blank = factory_->MakeFrame(kId);

  // Toggle all test points twice, and query them.
  for (const auto &pt : kPoints) {
    blank->ToggleCell(pt.first, pt.second);
    blank->ToggleCell(pt.first, pt.second);
    EXPECT_EQ(blank->QueryCell(pt.first, pt.second), CellState::kDead)
      << "Point (" << pt.first << ", " << pt.second << ") was still alive";
  }
}

TEST_P(FrameInterfaceTest, GetIdReturnsTheRightId) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  factory_->SetCellFactory(mock_cell_factory.get());
  auto blank = factory_->MakeFrame(kId);

  EXPECT_EQ(blank->GetId(), kId);
}

TEST_P(FrameInterfaceTest, LivingCellsEmptyCase) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  factory_->SetCellFactory(mock_cell_factory.get());
  auto blank = factory_->MakeFrame(kId);

  EXPECT_TRUE(blank->LivingCells().empty());
}

TEST_P(FrameInterfaceTest, LivingCellsSingleCell) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  factory_->SetCellFactory(mock_cell_factory.get());
  auto frame = factory_->MakeFrame(kId);

  frame->ToggleCell(1,2);

  auto cellList = frame->LivingCells();
  auto cellSet = std::set<std::pair<int,int>>(std::make_move_iterator(cellList.begin()), std::make_move_iterator(cellList.end()));
  std::set<std::pair<int,int>> expectedSet = {std::pair<int,int>(1,2)};

  EXPECT_EQ(cellSet, expectedSet);
}

TEST_P(FrameInterfaceTest, LivingCellsManyCells) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  factory_->SetCellFactory(mock_cell_factory.get());
  auto frame = factory_->MakeFrame(kId);

  frame->ToggleCell(1,2);
  frame->ToggleCell(5,3);
  frame->ToggleCell(100,10000);

  auto cellList = frame->LivingCells();
  auto cellSet = std::set<std::pair<int,int>>(std::make_move_iterator(cellList.begin()), std::make_move_iterator(cellList.end()));
  std::set<std::pair<int,int>> expectedSet = {std::pair<int,int>(1,2), std::pair<int,int>(5,3), std::pair<int,int>(100,10000)};

  EXPECT_EQ(cellSet, expectedSet);
}

TEST_P(FrameInterfaceTest, CreateFromListOfCells) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  factory_->SetCellFactory(mock_cell_factory.get());

  std::vector<std::pair<int, int>> inputData(3);
  std::pair<int, int> pair0(32, 32);
  std::pair<int, int> pair1(64, 64);
  std::pair<int, int> pair2(16, 16);
  inputData.at(0) = pair0;
  inputData.at(1) = pair1;
  inputData.at(2) = pair2;

  auto frame = factory_->MakeFrame(0, inputData);

  EXPECT_THAT(inputData, UnorderedElementsAreArray(frame->LivingCells()));
}

TEST_P(FrameInterfaceTest, ComputeNextFrame_SoloCell) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  EXPECT_CALL(*mock_cell_factory, MakeCell(_, _, _)).Times(1);
  factory_->SetCellFactory(mock_cell_factory.get());

  // Create an initial (blank) frame.
  auto first_frame = factory_->MakeFrame(kId);
  EXPECT_EQ(first_frame->LivingCells().size(), 0);

  // Set the cell at (1, 1) to be alive.
  first_frame->SetCellAlive(1, 1);
  EXPECT_EQ(first_frame->LivingCells().size(), 1);

  // Compute the next frame.
  auto second_frame = factory_->MakeFrame(first_frame.get());
  EXPECT_EQ(second_frame->GetId(), kId + 1);
  EXPECT_EQ(second_frame->LivingCells().size(), 0);
}

TEST_P(FrameInterfaceTest, ComputeNextFrame_OneNeighbor) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  EXPECT_CALL(*mock_cell_factory, MakeCell(_, _, _)).Times(2);
  factory_->SetCellFactory(mock_cell_factory.get());

  // Create an initial (blank) frame.
  auto first_frame = factory_->MakeFrame(kId);
  EXPECT_EQ(first_frame->LivingCells().size(), 0);

  // Set the cells at (1, 1) and (1, 2) to be alive.
  first_frame->SetCellAlive(1, 1);
  first_frame->SetCellAlive(1, 2);
  EXPECT_EQ(first_frame->LivingCells().size(), 2);

  // Compute the next frame.
  auto second_frame = factory_->MakeFrame(first_frame.get());
  EXPECT_EQ(second_frame->GetId(), kId + 1);
  EXPECT_EQ(second_frame->LivingCells().size(), 0);
}

TEST_P(FrameInterfaceTest, ComputeNextFrame_TwoNeighbors) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  EXPECT_CALL(*mock_cell_factory, MakeCell(_, _, _)).Times(6);
  factory_->SetCellFactory(mock_cell_factory.get());

  // Create an initial (blank) frame.
  auto first_frame = factory_->MakeFrame(kId);
  EXPECT_EQ(first_frame->LivingCells().size(), 0);

  // Set the cells {(1,1), (1,2), (1,3)} to be alive.
  first_frame->SetCellAlive(1, 1);
  first_frame->SetCellAlive(1, 2);
  first_frame->SetCellAlive(1, 3);
  EXPECT_EQ(first_frame->LivingCells().size(), 3);

  // Compute the second frame.
  auto second_frame = factory_->MakeFrame(first_frame.get());
  EXPECT_EQ(second_frame->GetId(), kId + 1);
  EXPECT_EQ(second_frame->LivingCells().size(), 3);
}

TEST_P(FrameInterfaceTest, ComputeNextFrame_FourNeighbors) {
  auto mock_cell_factory = MakeDefaultMockCellFactory();
  EXPECT_CALL(*mock_cell_factory, MakeCell(_, _, _)).Times(21);
  factory_->SetCellFactory(mock_cell_factory.get());

  // Create an initial (blank) frame.
  auto first_frame = factory_->MakeFrame(kId);
  EXPECT_EQ(first_frame->LivingCells().size(), 0);

  // Set the cells {(1, 1), (0, 1), (2, 1), (1, 0), (1, 2)} to be alive
  first_frame->SetCellAlive(1, 1);
  first_frame->SetCellAlive(0, 1);
  first_frame->SetCellAlive(2, 1);
  first_frame->SetCellAlive(1, 0);
  first_frame->SetCellAlive(1, 2);
  EXPECT_EQ(first_frame->LivingCells().size(), 5);

  // Compute the next frame.
  auto second_frame = factory_->MakeFrame(first_frame.get());
  EXPECT_EQ(second_frame->GetId(), kId + 1);
  EXPECT_EQ(second_frame->LivingCells().size(), 8);

  // Compute the third frame.
  auto third_frame = factory_->MakeFrame(second_frame.get());
  EXPECT_EQ(third_frame->GetId(), kId + 2);
  EXPECT_EQ(third_frame->LivingCells().size(), 8);
}

}  // anonymous namespace
}  // namespace execution
