#include "execution/tests/world.h"
#include "execution/tests/mock_objlib_dispatcher.h"

#include <memory>
#include <utility>
#include "gmock/gmock.h"

using ::testing::ByMove;
using ::testing::Return;
using ::testing::Invoke;
using ::testing::UnorderedElementsAreArray;
using ::testing::_;
using ::testing::Eq;
using ::testing::StrEq;
using ::testing::A;
using ::testing::An;
using ::testing::DoDefault;


using namespace ol;

// These are the tests for the WorldInterfaceTest fixture.
namespace execution {

const int WorldInterfaceTest::kId;
constexpr char WorldInterfaceTest::kTestName[];

namespace {
TEST_P(WorldInterfaceTest, SetName) {
  auto mock_frame_factory = MakeMockFrameFactory();

  auto world = factory_->MakeWorld(kTestName, std::move(mock_frame_factory));

  EXPECT_EQ(world->GetName(), kTestName);
}

TEST_P(WorldInterfaceTest, EmptyWorld) {
  auto mock_frame_factory = MakeMockFrameFactory();
  
  auto world = factory_->MakeWorld(kTestName, std::move(mock_frame_factory));

  EXPECT_EQ(world->CountFrames(), 0);
}

TEST_P(WorldInterfaceTest, GetBlankFrameIncreasesCountByOne) {
  auto mock_frame_factory = MakeMockFrameFactory();

  // Expect call to blank frame factory method.
  std::unique_ptr<MockFrame> mock_frame(new MockFrame);
  MockFrame *returned_frame = mock_frame.get();
  EXPECT_CALL(*mock_frame_factory, MakeFrame(kId))
    .WillOnce(Return(ByMove(std::move(mock_frame))));

  auto world = factory_->MakeWorld(kTestName, std::move(mock_frame_factory));

  EXPECT_EQ(world->GetBlankFrame(kId), returned_frame);
  EXPECT_EQ(world->CountFrames(), 1);
}

TEST_P(WorldInterfaceTest, GetFrameByIdReturnsCorrectFrame) {
  auto mock_frame_factory = MakeMockFrameFactory();

  // Just return a mock frame.
  EXPECT_CALL(*mock_frame_factory, MakeFrame(kId))
    .WillOnce(Return(ByMove(std::unique_ptr<MockFrame>(new MockFrame))));

  auto world = factory_->MakeWorld(kTestName, std::move(mock_frame_factory));

  // Make new blank frame.
  auto *frame = world->GetBlankFrame(kId);
  // Expect that we can get it by id.
  EXPECT_EQ(world->GetFrameById(kId), frame);
}

TEST_P(WorldInterfaceTest, GetFrameByIdReturnsNullForBadId) {
  auto mock_frame_factory = MakeMockFrameFactory();

  auto world = factory_->MakeWorld(kTestName, std::move(mock_frame_factory));

  // World is blank, so this fetch should fail.
  EXPECT_EQ(world->GetFrameById(kId), nullptr);
}

TEST_P(WorldInterfaceTest, GetFrameAfterInvokesCalculationWhenMissing) {
  auto mock_frame_factory = MakeMockFrameFactory();
  
  // Expect a call to blank frame, and next frame factory metods.
  std::unique_ptr<MockFrame> mock_frame1(new MockFrame);
  std::unique_ptr<MockFrame> mock_frame2(new MockFrame);
  MockFrame *blank = mock_frame1.get();
  MockFrame *successor = mock_frame2.get();
  // Blank Frame Call
  EXPECT_CALL(*mock_frame_factory, MakeFrame(kId))
    .WillOnce(Return(ByMove(std::move(mock_frame1))));

  // Next Frame Call
  EXPECT_CALL(*mock_frame_factory, MakeFrame(blank))
    .WillOnce(Return(ByMove(std::move(mock_frame2))));

  auto world = factory_->MakeWorld(kTestName, std::move(mock_frame_factory));

  // Insert blank frame.
  world->GetBlankFrame(kId);
  // Now get succ.
  EXPECT_EQ(world->GetFrameAfter(kId), successor);
}

TEST_P(WorldInterfaceTest, GetFrameAfterInvokesCalculationWhenMissingRecursively) {
auto mock_frame_factory = MakeMockFrameFactory();

// Expect a call to blank frame, and next frame factory metods.
std::unique_ptr<MockFrame> mock_frame1(new MockFrame);
std::unique_ptr<MockFrame> mock_frame2(new MockFrame);
std::unique_ptr<MockFrame> mock_frame3(new MockFrame);
MockFrame *blank = mock_frame1.get();
MockFrame *successor1 = mock_frame2.get();
MockFrame *successor2 = mock_frame3.get();

// Blank Frame Call
EXPECT_CALL(*mock_frame_factory, MakeFrame(kId))
.WillOnce(Return(ByMove(std::move(mock_frame1))));
// Next Frame Call
EXPECT_CALL(*mock_frame_factory, MakeFrame(blank))
.WillOnce(Return(ByMove(std::move(mock_frame2))));
// the frame after that
EXPECT_CALL(*mock_frame_factory, MakeFrame(successor1))
.WillOnce(Return(ByMove(std::move(mock_frame3))));

auto world = factory_->MakeWorld(kTestName, std::move(mock_frame_factory));

// Insert blank frame.
world->GetBlankFrame(kId);
// this should now calculate both missing frames recursively
EXPECT_EQ(world->GetFrameById(kId + 2), successor2);
}

TEST_P(WorldInterfaceTest, GetFrameAfterReturnsExistentFrameIfPresent) {
  auto mock_frame_factory = MakeMockFrameFactory();

  // Expect two blank calls.
  EXPECT_CALL(*mock_frame_factory, MakeFrame(kId))
    .WillOnce(Return(ByMove(std::unique_ptr<MockFrame>(new MockFrame))));
  EXPECT_CALL(*mock_frame_factory, MakeFrame(kId + 1))
    .WillOnce(Return(ByMove(std::unique_ptr<MockFrame>(new MockFrame))));

  auto world = factory_->MakeWorld(kTestName, std::move(mock_frame_factory));

  world->GetBlankFrame(kId);
  auto *successor = world->GetBlankFrame(kId + 1);

  EXPECT_EQ(world->GetFrameAfter(kId), successor);
}

TEST_P(WorldInterfaceTest, GetFrameAfterReturnsNullptrWhenPredecessorMissing) {
  auto mock_frame_factory = MakeMockFrameFactory();

  auto world = factory_->MakeWorld(kTestName, std::move(mock_frame_factory));

  EXPECT_EQ(world->GetFrameAfter(kId), nullptr);
}

TEST_P(WorldInterfaceTest, GetFrameByIdChecksObjLib) {
  auto mock_frame_factory = MakeMockFrameFactory();
  auto *mock_dispatcher = new MockObjDispatcher;


  std::vector<std::pair<int,int>> saved_frame_data = {std::make_pair(1,2), std::make_pair(3,4), std::make_pair(5,6)};

  // when objlib is called, place the mock data into the frame
  EXPECT_CALL(*mock_dispatcher, Single(ol::GET, ol::FRAME, kId, StrEq(kTestName), _))
    .WillOnce(Invoke([saved_frame_data](Ops ops,
                        Obj_Type obj,
                        int frame_name,
                        std::string world_name,
                        std::vector< std::pair<int, int> > *data) {
      *data = saved_frame_data;
      return 0;
    }));

  //EXPECT_CALL(*mock_frame_factory, MakeFrame(kId, A<std::vector<std::pair<int,int>>>()))
  //.WillOnce(DoDefault());
  //.WillOnce(Invoke([](int id, std::vector<std::pair<int, int>> grid){
  //  return std::unique_ptr<FrameInterface>(
  //    new HashMapFrame(id, nullptr, nullptr, nullptr, nullptr, grid));
  //  }));

  ON_CALL(*mock_frame_factory, MakeFrame(An<int>(), A<std::vector<std::pair<int,int>>>()))
  .WillByDefault(Invoke(mock_frame_factory.get(), &MockFrameFactory::MakeFrameConcrete));

  auto world = factory_->MakeWorld(kTestName, std::move(mock_frame_factory), mock_dispatcher);

  FrameInterface *loaded_frame = world->GetFrameById(kId);

  EXPECT_THAT(loaded_frame->LivingCells(), UnorderedElementsAreArray(saved_frame_data));
}


}  // anonymous namespace
}  // namespace execution
