/*!
 * \file parsing.cpp
 *
 */

#include "integer.hpp"
using namespace cyy::math;
namespace {

  extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    try {
      if (Size == 0) {
        return 0;
      }
      cyy::math::integer(
          std::string(reinterpret_cast<const char *>(Data), Size));
    } catch (const std::exception &) {
    }
    return 0; // Non-zero return values are reserved for future use.
  }
} // namespace
