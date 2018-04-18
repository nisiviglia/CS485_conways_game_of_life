#include "execution/impl/simple_world.h"
#include "../include/execution/impl/simple_world.h"
#include "../include/execution/interfaces/frame.h"

namespace execution {
FrameInterface *SimpleWorld::GetFrameById(int id) {

  if(id < 0) {
    // this may occur when calling GetFrameById(id - 1) recursively, if no frame 0 exists
    return nullptr;
  }

  // find the frame in the map
  auto entry = frames_.find(id);
  if (entry != frames_.end())
    return entry->second.get();


  if(dispatcher_ != nullptr) {
    // if the frame is not in the map, try to find it in objlib
    std::vector < std::pair < int, int >> outputData = std::vector < std::pair < int, int >> (0);
    int rc = dispatcher_->Single(
        ol::GET, ol::FRAME, id, this->GetName(), &outputData);

    if (rc == 0) {
      // we got the frame successfully, add to local map and return it

      // create a new world with this data and id
      std::unique_ptr<FrameInterface> newFrame = frame_factory_->MakeFrame(id, outputData);

      FrameInterface *ref = newFrame.get();

      frames_.emplace(id, std::move(newFrame));

      // return a raw pointer to the frame which is now owned by this world
      return ref;
    }
  }

  // the frame is not in the objlib, we must calculate it

  // get the previous frame
  FrameInterface *prevFrame = GetFrameById(id - 1);

  if(prevFrame == nullptr) {
    // there is no predecessor for this frame!
    return nullptr;
  }

  // calculate the current frame from the previous frame
  std::unique_ptr<FrameInterface> newFrame = frame_factory_->MakeFrame(prevFrame);

  if(dispatcher_ != nullptr) {
    // add this new frame to the objlib
    std::vector<std::pair<int, int>> newFrameLivingCells = newFrame->LivingCells();

    int rc = dispatcher_->Single(
        ol::INSERT, ol::FRAME, id, this->GetName(), &newFrameLivingCells);
  }

  FrameInterface *calculated = newFrame.get();

  // cache this frame in memory
  frames_.emplace(id, std::move(newFrame));

  return calculated;

}

FrameInterface *SimpleWorld::GetFrameAfter(int id) {
  return GetFrameById(id + 1);
}

FrameInterface *SimpleWorld::GetBlankFrame(int id) {
  // Make pair of id and new frame.
  auto elt = std::make_pair(id, frame_factory_->MakeFrame(id));

  // Try to insert.
  auto ret = frames_.insert(std::move(elt));
  if (!ret.second) {  // Insertion failed.
    return nullptr;
  }


  return ret.first->second.get();
}

//TODO this should probably invalidate future frames
FrameInterface *SimpleWorld::MakeFrameWithIdFromCells(int id, std::vector<std::pair<int,int>> cells) {
  auto newFrame = frame_factory_->MakeFrame(id, cells);
  auto *ref = newFrame.get();
  auto the_cells = ref->LivingCells();

  // add this frame to the local map
  frames_[id] = std::move(newFrame);

  // add this frame to the objlib TODO this will fail if the frame aleady exists
  int rc = dispatcher_->Single(
      ol::INSERT, ol::FRAME, id, this->GetName(), &the_cells);

  return ref;
}

}  // namespace execution
