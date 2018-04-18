#ifndef EXECUTION_INTERFACES_FRAME_H_
#define EXECUTION_INTERFACES_FRAME_H_

#include <memory>
#include <utility>
#include <vector>

#include "execution/interfaces/cell.h"
#include "execution/interfaces/world.h"

namespace execution {
// Forward Declarations.
enum class CellState;
class CellFactoryInterface;
class WorldInterface;

class FrameInterface {
public:

  FrameInterface() = default;

  // Virtual destructor
  virtual ~FrameInterface() = default;
  
  // Get a reference to the next frame in the sequence.
  virtual FrameInterface *Next() const = 0;

  // Get a reference to the previous frame in the sequence.
  virtual FrameInterface *Prev() const = 0;

  virtual void SetNext(FrameInterface *next) = 0;
  virtual void SetPrev(FrameInterface *prev) = 0;

  // Toggle the state of a specific cell.
  virtual void ToggleCell(int x, int y) = 0;
  virtual void ToggleCell(std::pair<int, int> pt) = 0;

  // Create a cell.
  virtual void SetCellAlive(int x, int y) = 0;
  virtual void SetCellAlive(std::pair<int, int> pt) = 0;

  // Query the state of a specific cell.
  virtual CellState QueryCell(int x, int y) const = 0;
  virtual CellState QueryCell(std::pair<int, int> pt) const = 0;

  virtual std::vector<std::pair<int,int>> LivingCells() const = 0;

  virtual int CountLivingNeighbors(std::pair<int, int> pt) const = 0;

  // Return the sequential ID number of the current frame.
  virtual int GetId() const = 0;

  // Return a reference to the world in which the frame resides.
  virtual WorldInterface *GetParent() const = 0;
};

class FrameFactoryInterface {
public:
  // Virtual Destructor
  virtual ~FrameFactoryInterface() = default;

  // Retrieves the World this frame factory belongs to.
  virtual WorldInterface *GetParent() const = 0;
  // Sets the world this frame factory belongs to.
  virtual void SetParent(WorldInterface *parent) = 0;

  // Retrieves the Cell Factory this frame factory gives to its children.
  virtual CellFactoryInterface *GetCellFactory() const = 0;
  // Sets the cell factory this frame gives to its children.
  virtual void SetCellFactory(CellFactoryInterface *) = 0;

  // Generates a new blank frame with the given ID and cell_factory.
  virtual std::unique_ptr<FrameInterface> MakeFrame(int id) = 0;
  
  // Generates a new frame based on it's previous frame and a given
  // cell_factory. This generation follows the rules of Conways Game of Life.
  // NOTE: This method forces a calculation, and remaps next of the predecessor
  //       to the new frame.
  virtual std::unique_ptr<FrameInterface> MakeFrame(FrameInterface *prev) = 0;

  // Create a new frame with the given ID and the given list of cells
  virtual std::unique_ptr<FrameInterface> MakeFrame(int id, std::vector<std::pair<int, int>> grid) = 0;
};

}  // namespace execution

#endif // EXECUTION_INTERFACES_FRAME_H_
