/*!
 * \file prime_test.cpp
 *
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_NO_EXCEPTIONS
#include <doctest/doctest.h>
#include <vector>

#include "../src/prime.hpp"
using namespace std;
using namespace cyy::math;

TEST_CASE("at") {
  std::vector<uint64_t> test_primes = {2,  3,  5,  7,  11, 13, 17, 19, 23,
                                       29, 31, 37, 41, 43, 47, 53, 59, 61,
                                       67, 71, 73, 79, 83, 89, 97};

  for (size_t i = 0; i < test_primes.size(); i++) {
    CHECK_EQ(primes().at(i), test_primes[i]);
  }
}

TEST_CASE("all") {
  std::vector<uint64_t> test_primes = {2,  3,  5,  7,  11, 13, 17, 19, 23,
                                       29, 31, 37, 41, 43, 47, 53, 59, 61,
                                       67, 71, 73, 79, 83, 89, 97};

  size_t i = 0;
  for (auto prime : primes().all()) {
    CHECK_EQ(prime, test_primes[i]);
    i++;
    if (i >= test_primes.size()) {
      break;
    }
  }
}
