#include "execution/impl/simple_world.h"

#include "execution/interfaces/world.h"
#include "execution/tests/world.h"

namespace execution {
namespace {
WorldFactoryInterface *GetSimpleWorldFactory() {
  // Static function initializers run at most once in C++11 (even among threads)
  static auto *factory = new SimpleWorldFactory;
  return factory;
}

INSTANTIATE_TEST_CASE_P(Simple, WorldInterfaceTest,
                        ::testing::Values(GetSimpleWorldFactory()));
}  // anonymous namespace
}  // namespace execution
