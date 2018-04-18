#ifndef EXECUTION_INTERFACES_CELL_H_
#define EXECUTION_INTERFACES_CELL_H_

#include <memory>
#include <utility>

#include "execution/interfaces/frame.h"

namespace execution {
// Forward Declarations.
class FrameInterface;

// A concrete type for the state of a cell.
// This may be swapped out for something more interesting in the future.
enum class CellState {
  kDead  = 0,
  kAlive,
};

class CellInterface {
public:
  // Virtual destructor
  virtual ~CellInterface() = default;
  
  // Retrieve the current cell's position on the grid.
  virtual int GetPositionX() const = 0;
  virtual int GetPositionY() const = 0;
  virtual std::pair<int, int> GetPosition() const = 0;
  
  // Get this cell's current state.
  virtual CellState GetState() const = 0;
  // Gets a binary view of the cell's current state.
  virtual bool IsAlive() const = 0;

  // Get the parent frame for the current cell.
  virtual FrameInterface *GetParent() const = 0;
};

class CellFactoryInterface {
public:
  // Virtual destructor
  virtual ~CellFactoryInterface() = default;

  // Construct a cell interface from it's position and it's state.
  virtual std::unique_ptr<CellInterface> MakeCell(int x, int y,
                                                  CellState state,
                                                  FrameInterface *parent) = 0;
  virtual std::unique_ptr<CellInterface> MakeCell(std::pair<int, int> pt,
                                                  CellState state,
                                                  FrameInterface *parent) = 0;
};

}  // namespace exeuction

#endif // EXECUTION_INTERFACES_CELL_H_
