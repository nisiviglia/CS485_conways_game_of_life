#include "execution/impl/hash_map_frame.h"

#include "execution/interfaces/frame.h"
#include "execution/tests/frame.h"

namespace execution {
namespace {
// Constructs a hash map frame factory at most once. Has life-time of all tests,
// so the resource is never deallocated.
FrameFactoryInterface *GetHashMapFrameFactory() {
  static FrameFactoryInterface *factory = new HashMapFrameFactory;
  return factory;
}

// Instantiation of the FrameInterfaceTests for HashMapFrames.
INSTANTIATE_TEST_CASE_P(HashMap, FrameInterfaceTest,
                        ::testing::Values(GetHashMapFrameFactory()));
}  // anonymous namespace
}  // namespace execution
