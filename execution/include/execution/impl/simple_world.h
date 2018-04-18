#ifndef EXECUTION_IMPL_SIMPLE_WORLD_H_
#define EXECUTION_IMPL_SIMPLE_WORLD_H_

#include "execution/interfaces/world.h"

#include <memory>
#include <unordered_map>
#include <utility>

#include "execution/interfaces/frame.h"


namespace execution {
// Simple implementation for world.
class SimpleWorld : public WorldInterface {
 public:
  SimpleWorld(const char *name,
              std::unique_ptr<FrameFactoryInterface> frame_factory,
              ol::ObjDispatcher *dispatcher)
    : name_(name),
      frame_factory_(std::move(frame_factory)),
      dispatcher_(dispatcher){}

  // Disallow Copy and Assign
  SimpleWorld(const SimpleWorld&) = delete;
  SimpleWorld& operator=(const SimpleWorld&) = delete;

  ~SimpleWorld() override = default;

  const char *GetName() const override {
    return name_;
  }

  int CountFrames() const override {
    return frames_.size();
  }

  FrameInterface *GetFrameById(int id) override;
  FrameInterface *GetFrameAfter(int id) override;
  FrameInterface *GetBlankFrame(int id) override;
  FrameInterface *MakeFrameWithIdFromCells(int id, std::vector<std::pair<int,int>>) override;

 private:
  // This world's name.
  const char *name_;
  // Frame Storage, maps frame ids to frames.
  std::unordered_map<int, std::unique_ptr<FrameInterface>> frames_;

  // Factories for constructing child frames.
  std::unique_ptr<FrameFactoryInterface> frame_factory_;

  // dispatcher for the objlib
  ol::ObjDispatcher *dispatcher_;
};

// World factory implementation that constructs SimpleWorlds
class SimpleWorldFactory : public WorldFactoryInterface {
 public:
  ~SimpleWorldFactory() override = default;

  std::unique_ptr<WorldInterface> MakeWorld(
      const char *name,
      std::unique_ptr<FrameFactoryInterface> frame_factory,
      ol::ObjDispatcher *dispatcher = nullptr) override {
    return std::unique_ptr<WorldInterface>(new SimpleWorld(
          name, std::move(frame_factory), dispatcher));
  }
};
}  // namespace execution

#endif  // EXECUTION_IMPL_SIMPLE_WORLD_H_
