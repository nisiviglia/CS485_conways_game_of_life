#ifndef EXECUTION_IMPL_SIMPLE_CELL_H_
#define EXECUTION_IMPL_SIMPLE_CELL_H_

#include "execution/interfaces/cell.h"

#include <memory>
#include <utility>

#include "execution/interfaces/frame.h"

namespace execution {

class SimpleCell : public CellInterface {
public:
  // Basic member constructor.
  SimpleCell(int x, int y, CellState state, FrameInterface *parent)
    : x_(x),
      y_(y),
      state_(state),
      parent_(parent) {}
  // Point constructor.
  SimpleCell(std::pair<int, int> pt, CellState state, FrameInterface *parent)
    : x_(pt.first),
      y_(pt.second),
      state_(state),
      parent_(parent) {}

  // Virtual destructor
  ~SimpleCell() override = default;

  // Position Getters
  int GetPositionX() const override { return x_; }
  int GetPositionY() const override { return y_; }
  std::pair<int, int> GetPosition() const override {
    return std::make_pair(x_, y_);
  }

  // State Getters
  CellState GetState() const override { return state_; }
  bool IsAlive() const override { return state_ != CellState::kDead; }

  // Parent Getter
  FrameInterface *GetParent() const override { return parent_; }

private:
  // Location of cell.
  int x_, y_;
  // State of cell.
  CellState state_;
  // Pointer to parent of cell.
  FrameInterface *parent_;
};

class SimpleCellFactory : public CellFactoryInterface {
public:
  // Virtual destructor
  ~SimpleCellFactory() override = default;

  std::unique_ptr<CellInterface> MakeCell(int x, int y,
                                          CellState state,
                                          FrameInterface *parent) override {
    return std::unique_ptr<CellInterface>(new SimpleCell(x, y, state, parent));
  }
  std::unique_ptr<CellInterface> MakeCell(std::pair<int, int> pt,
                                          CellState state,
                                          FrameInterface *parent) override {
    return std::unique_ptr<CellInterface>(new SimpleCell(pt, state, parent));
  }
};

}  // namespace execution

#endif  // EXECUTION_IMPL_SIMPLE_CELL_H_
