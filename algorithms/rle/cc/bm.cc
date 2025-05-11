/*
g++ -std=c++20 bm.cc -lbenchmark && ./a.out
g++ -std=c++20 bm.cc -lbenchmark && ./a.out --benchmark_filter='benchmark_basic<rle_gen_t>'

sudo apt install valgrind
valgrind --tool=massif ./a.out
ms_print massif.out.310
*/

#include "rle_iter.h"
#include "rle_gen.h"

#include <benchmark/benchmark.h>

#include <algorithm>
#include <numeric>
#include <random>


using namespace std;
using namespace benchmark;


vector<int> random_data() {
  auto xs = vector<int>(100000);

  iota(begin(xs), end(xs), 1);
  shuffle(begin(xs), end(xs), mt19937{random_device{}()});
  return xs; // NRVO
}

static const vector<int> xs = random_data();

template <class rle_t>
static void benchmark_basic(benchmark::State& state) {
  rle_t rle;
  for (auto _ : state) {
    rle(xs);
    // benchmark::DoNotOptimize(result);
  }
}

struct rle_iter_t {
  vector<pair<int, int>> operator()(vector<int> xs) {
    vector<pair<int, int>> ys;
    rle_iter::rle(begin(xs), end(xs), back_inserter(ys));
    return ys;
  }
};

struct rle_gen_t {
  vector<pair<int, int>> operator()(vector<int> xs) {
    vector<pair<int, int>> ys;
    auto dest = back_inserter(ys);
    for(auto x : rle_gen::rle(begin(xs), end(xs))) { 
      *dest = x;
    }
    return ys;
  }
};

BENCHMARK(benchmark_basic<rle_iter_t>);
BENCHMARK(benchmark_basic<rle_gen_t>);

BENCHMARK_MAIN();
