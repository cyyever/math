/*!
 * \file prime_test.cpp
 *
 */
#define DOCTEST_CONFIG_NO_EXCEPTIONS
#include <doctest/doctest.h>
#include <vector>

#include "../src/prime.hpp"
using namespace std;
using namespace cyy::math;

#ifndef _MSC_VER
TEST_CASE("all") {
  std::vector<uint64_t> test_primes = {2,  3,  5,  7,  11, 13, 17, 19, 23,
                                       29, 31, 37, 41, 43, 47, 53, 59, 61,
                                       67, 71, 73, 79, 83, 89, 97};

  size_t i = 0;
  for (auto prime : primes()) {
    CHECK_EQ(prime, test_primes[i]);
    i++;
    if (i >= test_primes.size()) {
      break;
    }
  }
}

TEST_CASE("from") {
  std::vector<uint64_t> test_primes = {11, 13, 17, 19, 23, 29, 31,
                                       37, 41, 43, 47, 53, 59, 61,
                                       67, 71, 73, 79, 83, 89, 97};

  auto prime_c = primes();
  size_t i = 0;
  for (auto prime : prime_c.from(10)) {
    CHECK_EQ(prime, test_primes[i]);
    i++;
    if (i >= test_primes.size()) {
      break;
    }
  }
}

TEST_CASE("till") {
  std::vector<uint64_t> test_primes = {2,  3,  5,  7,  11, 13, 17, 19, 23,
                                       29, 31, 37, 41, 43, 47, 53, 59, 61,
                                       67, 71, 73, 79, 83, 89, 97};

  auto prime_c = primes();
  size_t i = 0;
  for (auto prime : prime_c.till(100)) {
    CHECK_EQ(prime, test_primes[i]);
    i++;
    if (i >= test_primes.size()) {
      break;
    }
  }
}
#endif

#if !defined(_GLIBCXX_DEBUG)
TEST_CASE("has") {
  CHECK(primes().has(2));
  CHECK(!primes().has(10));
  CHECK(primes().has(982451653));
}
#endif
