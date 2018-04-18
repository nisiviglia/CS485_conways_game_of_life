#include "execution/impl/simple_cell.h"

#include "execution/interfaces/cell.h"
#include "execution/tests/cell.h"

namespace execution {
namespace {
// Constructs a simple cell factory at most once. Has a life-time of the entire
// tests, so the resource is never deallocated.
CellFactoryInterface *GetSimpleCellFactory() {
  // Static function initializers run at most once in C++11 (even among threads)
  static CellFactoryInterface *factory = new SimpleCellFactory;
  return factory;
}

// Instantiation of the CellInterfaceTests for SimpleCells by passing in a
// SimpleCellFactory instance by value.
INSTANTIATE_TEST_CASE_P(Simple, CellInterfaceTest,
                        ::testing::Values(GetSimpleCellFactory()));
}  // anonymous namespace
}  // namespace execution
