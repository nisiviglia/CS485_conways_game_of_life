#include "gtest/gtest.h"

#include <algorithm>
#include <iostream>
#include <vector>

// It's a good idea to put tests in anonymous namespaces.
namespace {
// The test macro takes a test case name, and a specific test name.
// The following two tests check that algorithm's min and max work.
TEST(AlgorithmExtremaTest, MinElementGetsTheMinimum) {
  std::vector<int> list = { 25, 17, 42, 420 };

  int min = *std::min_element(list.begin(), list.end());

  EXPECT_EQ(17, min);
}
TEST(AlgorithmExtremaTest, MaxElementGetsTheMaximum) {
  std::vector<int> list = { 25, 17, 42, 420 };

  int min = *std::max_element(list.begin(), list.end());

  EXPECT_EQ(420, min);
}

// The above two tests cases have some data in common, so it may have been
// useful to use a test fixture to share that data:

// A fixture should extend from ::testing::Test, and all it's members should be
// protected.
class BetterAlgorithmExtremaTest : public ::testing::Test {
 protected:
  // Called once before each test runs.
  BetterAlgorithmExtremaTest() {
    std::cout << "Setting up a test" << std::endl;
  }
  // Called once after each test runs.
  ~BetterAlgorithmExtremaTest() override {
    std::cout << "Cleaning up after a test" << std::endl;
  }

  // Called once before ALL tests in this case run.
  static void SetUpTestCase() {
    std::cout << "Setting up an entire test case" << std::endl;
  }
  // Called once after ALL tests in this case run.
  static void TearDownTestCase() {
    std::cout << "Tearing down an entire test case" << std::endl;
  }

  // Maybe shared helper methods are nice.
  int GetMax() {
    return *std::max_element(list_.begin(), list_.end());
  }

  int GetMin() {
    return *std::min_element(list_.begin(), list_.end());
  }

  // Maybe shared members are nice.
  std::vector<int> list_ = { 25, 17, 42, 420 };
};

// Tests that use a test fixture NEED to sure the TEST_F macro.
TEST_F(BetterAlgorithmExtremaTest, MinElementGetsTheMinimum) {
  EXPECT_EQ(GetMin(), 17);
}
TEST_F(BetterAlgorithmExtremaTest, MaxElementGetsTheMaximum) {
  EXPECT_EQ(GetMax(), 420);
}
}  // anonymous namespace

