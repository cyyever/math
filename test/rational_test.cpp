/*!
 * \file rational_test.cpp
 *
 * \brief 测试rational类
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "../src/rational.hpp"
using namespace std;
using namespace cyy::math;

TEST_CASE("from_string") {
  for (auto const &str : {"1/2", "-1/2", "5"}) {
    auto a = rational(str);
    CHECK_EQ(str, a.to_string());
  }
}
TEST_CASE("simplify") {
  CHECK_EQ(rational(-3, 7).simplify(), rational(-3, 7));
  CHECK_EQ(rational(-14, 77).simplify(), rational(-2, 11));
  CHECK_EQ(rational(-5, 5).simplify(), rational(-1, 1));
}
TEST_CASE("compare") {
  for (int64_t a = -1; a < 2; a++) {
    for (int64_t b = -1; b < 2; b++) {
      CHECK_EQ(rational(a) < rational(b), a < b);
    }
  }
}

TEST_CASE("+") {
  SUBCASE("signed") {
    for (int64_t a = static_cast<int64_t>(INT32_MIN) - 1;
         a < static_cast<int64_t>(INT32_MIN) + 1; a++) {
      for (int64_t b = static_cast<int64_t>(INT32_MIN) - 1;
           b < static_cast<int64_t>(INT32_MIN) + 1; b++) {
        CHECK_EQ(rational(a) + rational(b), a + b);
      }
    }
    for (int64_t a = -1; a < 2; a++) {
      for (int64_t b = -1; b < 2; b++) {
        CHECK_EQ(rational(a) + rational(b), a + b);
      }
    }
  }
  SUBCASE("unsigned") {
    for (uint64_t a = static_cast<uint64_t>(UINT32_MAX) - 1;
         a < static_cast<uint64_t>(UINT32_MAX) + 1; a++) {
      for (uint64_t b = static_cast<uint64_t>(UINT32_MAX) - 1;
           b < static_cast<uint64_t>(UINT32_MAX) + 1; b++) {
        CHECK_EQ(rational(a) + rational(b), (a + b));
      }
    }
  }
}

TEST_CASE("-") {
  SUBCASE("signed") {
    for (int64_t a = static_cast<int64_t>(INT32_MIN) - 1;
         a < static_cast<int64_t>(INT32_MIN) + 1; a++) {
      for (int64_t b = static_cast<int64_t>(INT32_MIN) - 1;
           b < static_cast<int64_t>(INT32_MIN) + 1; b++) {
        CHECK_EQ(rational(a) - rational(b), a - b);
      }
    }

    for (int64_t a = -1; a < 2; a++) {
      for (int64_t b = -1; b < 2; b++) {
        CHECK_EQ(rational(a) - rational(b), a - b);
      }
    }
  }
  SUBCASE("unsigned") {
    for (int64_t a = static_cast<int64_t>(UINT32_MAX) - 1;
         a < static_cast<int64_t>(UINT32_MAX) + 1; a++) {
      for (int64_t b = static_cast<int64_t>(UINT32_MAX) - 1;
           b < static_cast<int64_t>(UINT32_MAX) + 1; b++) {
        CHECK_EQ(rational(a) - rational(b), (a - b));
      }
    }
  }
}

TEST_CASE("*") {
  SUBCASE("signed") {
    for (int64_t a = static_cast<int64_t>(INT32_MIN) - 1;
         a < static_cast<int64_t>(INT32_MIN) + 1; a++) {
      for (int64_t b = static_cast<int64_t>(INT32_MIN) - 1;
           b < static_cast<int64_t>(INT32_MIN) + 1; b++) {
        CHECK_EQ(rational(a) * rational(b), (a * b));
      }
    }
  }
  SUBCASE("unsigned") {
    for (uint64_t a = static_cast<uint64_t>(UINT32_MAX) - 1;
         a < static_cast<uint64_t>(UINT32_MAX) + 1; a++) {
      uint64_t b = static_cast<uint64_t>(UINT32_MAX) - 1;
      CHECK_EQ(rational(a) * rational(b), (a * b));
    }
  }
}

