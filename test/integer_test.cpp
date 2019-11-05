/*
 *	程序名：integer_test.cpp
 *	作者：陈源源
 *	日期：2016-03-28
 *	功能：测试integer类
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "../src/integer.hpp"
using namespace std;
using namespace cyy::math;

/* TEST_CASE("from_string") { */
/*   SUBCASE("valid_string") { */
/*     std::vector<std::string> strs{"1", "-1", "0", to_string(UINT64_MAX), */
/*                                   to_string(INT64_MIN)}; */
/*     for (auto const &str : strs) { */
/*       auto a = integer(str); */
/*       CHECK_EQ(str, a.to_string()); */
/*     } */
/*   } */

/*   SUBCASE("invalid_string") { */
/*     for (auto const &str : {"aaa", "0123"}) { */
/*       CHECK_THROWS(auto a = integer(str)); */
/*     } */
/*   } */
/* } */
TEST_CASE("+") {
  SUBCASE("signed") {
    for (auto a = static_cast<int64_t>(INT32_MIN) - 1;
         a < static_cast<int64_t>(INT32_MIN) + 1; a++) {
      for (auto b = static_cast<int64_t>(INT32_MIN) - 1;
           b < static_cast<int64_t>(INT32_MIN) + 1; b++) {
        CHECK_EQ(integer(a) + integer(b), (a + b));
      }
    }
  }
  SUBCASE("unsigned") {
    for (auto a = static_cast<uint64_t>(UINT32_MAX) - 1;
         a < static_cast<uint64_t>(UINT32_MAX) + 1; a++) {
      for (auto b = static_cast<uint64_t>(UINT32_MAX) - 1;
           b < static_cast<uint64_t>(UINT32_MAX) + 1; b++) {
        CHECK_EQ(integer(a) + integer(b), (a + b));
      }
    }
  }
}

TEST_CASE("-") {
  SUBCASE("signed") {
    for (auto a = static_cast<int64_t>(INT32_MIN) - 1;
         a < static_cast<int64_t>(INT32_MIN) + 1; a++) {
      for (auto b = static_cast<int64_t>(INT32_MIN) - 1;
           b < static_cast<int64_t>(INT32_MIN) + 1; b++) {
        CHECK_EQ(integer(a) - integer(b), (a - b));
      }
    }
  }
  SUBCASE("unsigned") {
    for (auto a = static_cast<int64_t>(UINT32_MAX) - 1;
         a < static_cast<int64_t>(UINT32_MAX) + 1; a++) {
      for (auto b = static_cast<int64_t>(UINT32_MAX) - 1;
           b < static_cast<int64_t>(UINT32_MAX) + 1; b++) {
        CHECK_EQ(integer(a) - integer(b), (a - b));
      }
    }
  }
}
/* int main(int argc, char **argv) { */
/*   vector<string> invalid_interger_str = {"aaa", "0123"}; */
/*   vector<string> valid_interger_str = {"1", */
/*                                        "-1", */
/*                                        "0", */
/*                                        "-0", */
/*                                        "123456789012345678901", */
/*                                        "-12345678901234567890", */
/*                                        to_string(UINT64_MAX), */
/*                                        to_string(INT64_MIN)}; */

/*   for (auto it = invalid_interger_str.begin(); it !=
 * invalid_interger_str.end(); */
/*        it++) { */
/*     try { */
/*       integer a(*it); */
/*     } catch (invalid_argument &e) { */
/*     } */
/*   } */

/*   for (auto it = valid_interger_str.begin(); it !=
 * valid_interger_str.end(); */
/*        it++) { */
/*     integer a(*it); */
/*     assert(a == *it); */
/*   } */

/*   assert(integer(0).digit_num() == 1); */
/*   assert(integer(10).digit_num() == 2); */

/*   for (auto i = -200; i <= 200; i++) { */
/*     for (auto j = -200; j <= 200; j++) { */
/*       assert((i < j) == (integer(i) < integer(j))); */
/*       assert((i == j) == (integer(i) == integer(j))); */
/*       assert((i > j) == (integer(i) > integer(j))); */
/*       assert(i + j == (integer(i) + j)); */
/*       assert(i + j == (integer(i) + integer(j))); */
/*       assert(i - j == (integer(i) - j)); */
/*       assert(i - j == (integer(i) - integer(j))); */
/*       assert(i * j == (integer(i) * integer(j))); */
/*       if (j == 0) */
/*         continue; */
/*       assert(i / j == (integer(i) / j)); */
/*       assert(i / j == (integer(i) / integer(j))); */
/*       assert(i % j == (integer(i) % j)); */
/*       assert(i % j == (integer(i) % integer(j))); */
/*     } */
/*   } */
/*   assert(integer(INT64_MAX) + UINT64_MAX ==
 * string("27670116110564327422")); */
/*   assert(integer(INT64_MAX) - UINT64_MAX ==
 * string("-9223372036854775808")); */

/*   integer a; */
/*   a = 0; */
/*   assert(++a == 1); */
/*   assert(--a == 0); */
/*   assert((a++) == 0 && a == 1); */
/*   assert((a--) == 1 && a == 0); */

/*   a = 2; */
/*   assert(a * UINT64_MAX == string("36893488147419103230")); */
/*   assert(a * INT64_MIN == string("-18446744073709551616")); */

/*   return 0; */
/* } */
