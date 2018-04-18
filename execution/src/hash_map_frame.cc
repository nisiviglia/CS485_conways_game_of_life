#include "execution/impl/hash_map_frame.h"

namespace execution {
// Cell toggle implementations.
void HashMapFrame::ToggleCell(int x, int y) {
  ToggleCell(std::make_pair(x, y));
}

void HashMapFrame::ToggleCell(std::pair<int, int> pt) {
  // Attempt to insert a new alive cell into the grid.
  auto result = grid_.emplace(
      pt, cell_factory_->MakeCell(pt, CellState::kAlive, this));
  if (!result.second) {  // Insert failed.
    // This cell was already present in the grid, and therefore must be alive.
    // Therefore, we remove it from the grid.
    grid_.erase(result.first);
  }
}

// Cell setting implementations.
void HashMapFrame::SetCellAlive(int x, int y) {
  SetCellAlive(std::make_pair(x, y));
}

void HashMapFrame::SetCellAlive(std::pair<int, int> pt) {
  if (grid_.count(pt) > 0) return;
  grid_.emplace(pt, cell_factory_->MakeCell(pt, CellState::kAlive, this));
}

// Cell query implementations.
CellState HashMapFrame::QueryCell(int x, int y) const {
  return QueryCell(std::make_pair(x, y));
}

CellState HashMapFrame::QueryCell(std::pair<int, int> pt) const {
  auto entry = grid_.find(pt);
  if (entry == grid_.end()) {  // Lack of entry implies dead cell.
    return CellState::kDead;
  }

  return entry->second->GetState();
}

// Next frame computation implementation.
std::unique_ptr<FrameInterface> HashMapFrameFactory::MakeFrame(
    FrameInterface *prev) {

  std::unique_ptr<FrameInterface> next = MakeFrame(prev->GetId() + 1);
  std::vector<std::pair<int, int>> prev_state = prev->LivingCells();

  for (std::vector<std::pair<int, int>>::iterator it = prev_state.begin();
       it != prev_state.end(); it++) {

    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        std::pair<int, int> cell((*it).first + i, (*it).second + j);
        int adj_count = prev->CountLivingNeighbors(cell);

        if (adj_count == 2 && prev->QueryCell(cell) == CellState::kAlive) {
          next->SetCellAlive(cell);
        } else if (adj_count == 3) {
          next->SetCellAlive(cell);
        }
      }
    }
  }

  next->SetPrev(prev);
  prev->SetNext(next.get());

  return next;
}
}  // namespace execution
