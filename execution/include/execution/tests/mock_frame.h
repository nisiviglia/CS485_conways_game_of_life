#ifndef EXECUTION_TESTS_MOCK_FRAME_H_
#define EXECUTION_TESTS_MOCK_FRAME_H_

#include "execution/interfaces/frame.h"

#include "gmock/gmock.h"
#include "../impl/hash_map_frame.h"
#include "execution/tests/mock_cell.h"

namespace execution {
// These mocks are useful for unit testing.
// Should NOT be used in production code.
class MockFrame : public FrameInterface {
 public:
  ~MockFrame() override = default;

  MOCK_CONST_METHOD0(Next, FrameInterface *());
  MOCK_CONST_METHOD0(Prev, FrameInterface *());

  MOCK_METHOD1(SetNext, void(FrameInterface *));
  MOCK_METHOD1(SetPrev, void(FrameInterface *));
  
  MOCK_METHOD2(ToggleCell, void(int, int));
  MOCK_METHOD1(ToggleCell, void(std::pair<int, int>));

  MOCK_METHOD2(SetCellAlive, void(int, int));
  MOCK_METHOD1(SetCellAlive, void(std::pair<int, int>));

  MOCK_CONST_METHOD0(LivingCells, std::vector<std::pair<int,int>>());

  MOCK_CONST_METHOD1(CountLivingNeighbors, int(std::pair<int, int>));

  MOCK_CONST_METHOD2(QueryCell, CellState(int, int));
  MOCK_CONST_METHOD1(QueryCell, CellState(std::pair<int, int>));

  MOCK_CONST_METHOD0(GetId, int());

  MOCK_CONST_METHOD0(GetParent, WorldInterface *());
};

class MockFrameFactory : public FrameFactoryInterface {
 public:

  ~MockFrameFactory() override = default;

  MOCK_CONST_METHOD0(GetParent, WorldInterface *());
  MOCK_METHOD1(SetParent, void(WorldInterface *));

  MOCK_CONST_METHOD0(GetCellFactory, CellFactoryInterface *());
  MOCK_METHOD1(SetCellFactory, void(CellFactoryInterface *));

  MOCK_METHOD1(MakeFrame, std::unique_ptr<FrameInterface>(int id));
  MOCK_METHOD1(MakeFrame, std::unique_ptr<FrameInterface>(
      FrameInterface *prev));
  MOCK_METHOD2(MakeFrame, std::unique_ptr<FrameInterface>(
      int id, std::vector<std::pair<int, int>> grid));


  std::unique_ptr<FrameInterface> MakeFrameConcrete(int id, std::vector<std::pair<int, int>> grid) {
    return impl_.MakeFrame(id, grid);
  }

  void SetCellFactoryConcrete(CellFactoryInterface *f) {
    impl_.SetCellFactory(f);
  }

 private:
  HashMapFrameFactory impl_;

};
}  // namespace execution

#endif  // EXECUTION_TESTS_MOCK_FRAME_H_
