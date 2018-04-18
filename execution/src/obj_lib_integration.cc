#include "execution/impl/hash_map_frame.h"
#include "execution/tests/mock_cell.h"
#include "ObjDispatcher.hpp"
#include "../include/execution/interfaces/frame.h"

#include <iostream>
#include <string>
#include <iostream>
#include <vector>

using namespace execution;

int main(int argc, char *argv[]) {
  std::cout << "Hello, world" << std::endl;

  // create an empty frame
  FrameFactoryInterface *factory = new HashMapFrameFactory;
  MockCellFactory *cellFactory = new MockCellFactory;

  factory->SetCellFactory(cellFactory);

  std::unique_ptr<FrameInterface> myFrame = factory->MakeFrame(0);

  int rc = 0;
  //Crate dummy data
  int frameId = 10;
  std::string worldName = "test_world_name";
  std::vector<std::pair<int, int>> inputData(3);
  std::pair<int, int> pair0(32, 32);
  std::pair<int, int> pair1(64, 64);
  std::pair<int, int> pair2(16, 16);
  inputData.at(0) = pair0;
  inputData.at(1) = pair1;
  inputData.at(2) = pair2;

  for(auto const& cell: inputData) {
    myFrame->ToggleCell(cell);
  }

  //Create dispatcher
  ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();

  //Create dummy world
  rc = dispatcher->Single(ol::INSERT, ol::WORLD, 0, worldName, NULL);

  //Store data
  rc = dispatcher->Single(
      ol::INSERT, ol::FRAME, frameId, worldName, new std::vector<std::pair<int,int>>(myFrame->LivingCells())  );

  //Fetch data
  std::vector<std::pair<int, int>> outputData(0);
  rc = dispatcher->Single(
      ol::GET, ol::FRAME, frameId, worldName, &outputData);


  for (auto const &point: outputData) {
    std::cout << "(" << point.first << "," << point.second << ") ";
  }
  std::cout << std::endl;

  //Clean up
  delete (dispatcher);

}
