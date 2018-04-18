// This is an example file for making googletests with gmock.

// Normally the header for the component(s) you're testing would be included
// here.

// Gtest header.
#include "gtest/gtest.h"
// Gmock header.
#include "gmock/gmock.h"

#include <algorithm>
#include <functional>
#include <vector>

// Here we check std::sort works using matchers (my favorite gmock feature).
// NOTE: This file does not really exemplify gmock features (mocking interfaces)
//       because I'm too lazy to come up with an example. Instead, look at:
//       https://github.com/google/googletest/blob/master/googlemock/docs/ForDummies.md

using ::testing::ElementsAre;

namespace {
class AlgorithmSortTest : public ::testing::Test {
 protected:
  // Maybe shared members are nice.
  std::vector<int> list_ = { 25, 17, 42, 420 };
};

// Tests that use a test fixture NEED to sure the TEST_F macro.
TEST_F(AlgorithmSortTest, SortsByLess) {
  std::sort(list_.begin(), list_.end());

  EXPECT_THAT(list_, ElementsAre(17, 25, 42, 420));
}

TEST_F(AlgorithmSortTest, SortsByGreater) {
  std::sort(list_.begin(), list_.end(), std::greater<int>());

  EXPECT_THAT(list_, ElementsAre(420, 42, 25, 17));
}
}  // anonymous namespace
