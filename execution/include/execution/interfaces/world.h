#ifndef EXECUTION_INTERFACES_WORLD_H_
#define EXECUTION_INTERFACES_WORLD_H_

#include <memory>

#include "execution/interfaces/frame.h"
#include "ObjDispatcher.hpp"


namespace execution {
// Forward Declarations.
class FrameInterface;
class FrameFactoryInterface;

class WorldInterface {
public:
  // Virtual destructor
  virtual ~WorldInterface() = default;
  
  // Returns the name of this world (unique identifier).
  virtual const char *GetName() const = 0;

  // Return the number of frames in this current world.
  virtual int CountFrames() const = 0;

  // Return a specific frame by its sequence ID number.
  virtual FrameInterface *GetFrameById(int id) = 0;

  // Returns the frame directly after id by game sequence.
  virtual FrameInterface *GetFrameAfter(int id) = 0;

  // Constructs a new blank frame with the given id. Returns a pointer to the
  // new frame, but the world retains owernship.
  virtual FrameInterface *GetBlankFrame(int id) = 0;

  virtual FrameInterface *MakeFrameWithIdFromCells(int id, std::vector<std::pair<int,int>>) = 0;
};

class WorldFactoryInterface {
public:
 // Virtual destructor
 virtual ~WorldFactoryInterface() = default;

 // Makes a new world with a given name.
 virtual std::unique_ptr<WorldInterface> MakeWorld(
     const char *name,
     std::unique_ptr<FrameFactoryInterface> frame_factory,
     ol::ObjDispatcher *dispatcher = nullptr) = 0;

};
}  // namespace execution

#endif  // EXECUTION_INTERFACES_WORLD_H_
