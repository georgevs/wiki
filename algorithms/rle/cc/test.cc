/*
docker container start -ia dev-golang
cd ~/ws/NOTES/wiki/algorithms/rle/cc
g++ -std=c++20 test.cc -lgtest && ./a.out
g++ -std=c++20 test.cc -lgtest && ./a.out --gtest_filter=RleViewTest.*

valgrind --tool=massif --massif-out-file=massif.out ./a.out --gtest_filter=RleIterTest.test_big_data
valgrind --tool=massif --massif-out-file=massif.out ./a.out --gtest_filter=RleGenTest.test_big_data

ms_print massif.out
grep mem_heap_B massif.out | sort -t= -k2 -nr | head -n1
*/

#include "rle_gen.h"
#include "rle_iter.h"
#include "rle_view.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <utility>  // pair
#include <vector>

#include <gtest/gtest.h>

using namespace std;
using namespace testing;

vector<int> random_data() {
  auto xs = vector<int>(100000);

  iota(begin(xs), end(xs), 1);
  shuffle(begin(xs), end(xs), mt19937{random_device{}()});
  return xs; // NRVO
}

static const vector<int> xs = random_data();

class RleTest : public Test {
public:
  void test_basic(auto rle) {
    ASSERT_EQ((vector<pair<int, int>> {}), rle({}));
    ASSERT_EQ((vector<pair<int, int>> {{1,1}}), rle({1}));
    ASSERT_EQ((vector<pair<int, int>> {{1,1},{1,2},{1,3}}), rle({1,2,3}));
    ASSERT_EQ((vector<pair<int, int>> {{1,1},{2,2},{1,3},{3,4},{2,5}}), rle({1,2,2,3,4,4,4,5,5}));
  }
  void test_big_data(auto rle) {
    rle(xs);
  }
};

class RleIterTest : public RleTest {
public:
  struct rle_t {
    vector<pair<int, int>> operator()(vector<int> xs) {
      vector<pair<int, int>> ys;
      rle_iter::rle(begin(xs), end(xs), back_inserter(ys));
      return ys;
    }
  };
};

class RleGenTest : public RleTest {
public:
  struct rle_t {
    vector<pair<int, int>> operator()(vector<int> xs) {
      vector<pair<int, int>> ys;
      auto dest = back_inserter(ys);  // fancy generic ys.push_back(x) 
      for (auto x : rle_gen::rle(begin(xs), end(xs))) { 
        *dest = x;
      }
      return ys;
    }
  };
  struct rle_void_t {
    void operator()(vector<int> xs) {
      for (auto x : rle_gen::rle(begin(xs), end(xs))) { 
        // noop
      }
    }
  };
};

static_assert(ranges::input_range<rle_view::rle<vector<int>::iterator>>);

class RleViewTest : public RleTest {
public:
  struct rle_t {
    vector<pair<int, int>> operator()(vector<int> xs) {
      vector<pair<int, int>> ys;
      for (auto x : rle_view::rle(begin(xs), end(xs))) {
        ys.push_back(x);
      }
      return ys;
    }
  };
  struct rle_copy_t {
    vector<pair<int, int>> operator()(vector<int> xs) {
      vector<pair<int, int>> ys;
      rle_view::rle rs(begin(xs), end(xs));
      ranges::copy(begin(rs), end(rs), back_inserter(ys));
      return ys;
    }
  };
  struct rle_copy_range_t {
    vector<pair<int, int>> operator()(vector<int> xs) {
      vector<pair<int, int>> ys;
      ranges::copy(rle_view::rle(begin(xs), end(xs)), back_inserter(ys));
      return ys;
    }
  };
};

TEST_F(RleIterTest, test_basic) {
  test_basic(rle_t{});
}
TEST_F(RleIterTest, test_big_data) {
  test_big_data(rle_t{});
}

TEST_F(RleGenTest, test_basic) {
  test_basic(rle_t{});
}
TEST_F(RleGenTest, test_big_data) {
  test_big_data(rle_t{});
}

TEST_F(RleViewTest, test_basic) {
  test_basic(rle_t{});
}
TEST_F(RleViewTest, test_basic_copy) {
  test_basic(rle_copy_t{});
}
TEST_F(RleViewTest, test_basic_copy_range) {
  test_basic(rle_copy_range_t{});
}
TEST_F(RleViewTest, test_big_data) {
  test_big_data(rle_t{});
}

int main(int argc, char** argv) {
  InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
