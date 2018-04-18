#ifndef EXECUTION_IMPL_HASH_MAP_FRAME_H_
#define EXECUTION_IMPL_HASH_MAP_FRAME_H_

#include "execution/interfaces/frame.h"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>

#include "execution/interfaces/cell.h"
#include "execution/interfaces/world.h"
#include "../interfaces/frame.h"

#include <iostream>

namespace execution { 
// Functor for computing the hash of a pair of ints (point coordinate).
struct PointHash {
  size_t operator()(std::pair<int, int> point) const {
    return hasher(static_cast<long long>(point.first) |
                  (static_cast<long long>(point.second) << 32));
  }

  std::hash<long long> hasher;
};

// Frame implementation using std::unordered_map for storing cells.
class HashMapFrame : public FrameInterface {
public:
  // Type for internal cell storage (using directive is for clarity).
  // A Grid owns the cells it stores.
  using Grid = std::unordered_map<std::pair<int, int>,
                                  std::unique_ptr<CellInterface>,
                                  PointHash>;

  // Constructor requires a frame id, a parent, and a cell factory to construct
  // cells internally. Optionally takes previous and next pointers.
  HashMapFrame(int id, WorldInterface *parent,
               CellFactoryInterface *cell_factory,
               FrameInterface *prev = nullptr,
               FrameInterface *next = nullptr,
               std::vector<std::pair<int,int>> grid = std::vector<std::pair<int,int>>())
    : id_(id),
      parent_(parent),
      cell_factory_(cell_factory),
      prev_(prev),
      next_(prev) {

      for(auto const& point: grid) {
        this->ToggleCell(point);
      }
  }



  // Virtual destructor
  ~HashMapFrame() override = default;

  // Link Getters
  FrameInterface *Prev() const override { return prev_; }
  FrameInterface *Next() const override { return next_; }

  // Link Setters
  void SetPrev(FrameInterface *prev) override { this->prev_ = prev; }
  void SetNext(FrameInterface *next) override { this->next_ = next; }

  // Toggle cell.
  void ToggleCell(int x, int y) override;
  void ToggleCell(std::pair<int, int> pt) override;

  // Set the state of a cell.
  void SetCellAlive(int x, int y) override;
  void SetCellAlive(std::pair<int, int> pt) override;

  // Query cells.
  CellState QueryCell(int x, int y) const override;
  CellState QueryCell(std::pair<int, int> pt) const override;

  std::vector<std::pair<int,int>> LivingCells() const override {
    // a cell is alive if it is a key in the map

    // put all the keys in a vector
    std::vector<std::pair<int,int>> cellList;
    for(const auto & cell : grid_) {
      cellList.push_back(cell.first);
    }

    return cellList;
  }

  int CountLivingNeighbors(std::pair<int, int> pt) const override {
    int count = 0;

    if (this->grid_.empty()) return 0;
    
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        if (i == 0 && j == 0) continue;
        
        std::pair<int, int> pair(pt.first + i, pt.second + j);
        
        if (this->grid_.count(pair) > 0) {
          count += 1;
        }
      }
    } 

    return count;
  }

  // Get id of frame.
  int GetId() const override { return id_; }

  WorldInterface *GetParent() const override { return parent_; }

private:
  // Unique Identifier for this frame.
  int id_;
  // Pointer to parent world.
  WorldInterface *parent_;
  // Cell factory used for constructing cells.
  CellFactoryInterface *cell_factory_;
  // Links to previous and next frame.
  FrameInterface *next_, *prev_;
  // Cell Storage.
  Grid grid_;
};

// Factory implementation for the HashMapFrame.
class HashMapFrameFactory : public FrameFactoryInterface {
 public:
  // Virtual destructor
  ~HashMapFrameFactory() override = default;

  // Parent setter/accessor.
  WorldInterface *GetParent() const override { return parent_; }
  void SetParent(WorldInterface *parent) override { parent_ = parent; }

  // Cell Factory setter/accessor.
  CellFactoryInterface *GetCellFactory() const override {
    return cell_factory_;
  }
  void SetCellFactory(CellFactoryInterface *cell_factory) override {
    cell_factory_ = cell_factory;
  }

  // Constructs a new blank frame.
  virtual std::unique_ptr<FrameInterface> MakeFrame(int id) override {
    return std::unique_ptr<FrameInterface>(
        new HashMapFrame(id, parent_, cell_factory_));
  }

  std::unique_ptr<FrameInterface> MakeFrame(FrameInterface *prev) override;


  virtual std::unique_ptr<FrameInterface> MakeFrame(int id, std::vector<std::pair<int, int>> grid) override {
    std::cout << "MakeFrame called with existing data" << std::endl;
    return std::unique_ptr<FrameInterface>(
      new HashMapFrame(id, parent_, cell_factory_, nullptr, nullptr, grid));
  }


 private:
  // Pointer to parent.
  WorldInterface *parent_ = nullptr;
  // Pointer to cell factory.
  CellFactoryInterface *cell_factory_ = nullptr;
};

}  // namespace execution

#endif  // EXECUTION_IMPL_HASH_MAP_FRAME_H_
