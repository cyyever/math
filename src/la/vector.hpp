/*!
 * \file vector.hpp
 *
 * \brief vector related classes
 */
#pragma once

#include "../exception.hpp"
#include "../numerical_type.hpp"

namespace cyy::math::la {

  template <typename T,
            typename = std::enable_if_t<cyy::math::type::is_numerical_type<T>>>
  class vector_view final {
  public:
    using element_type = T;
    vector_view(T *_data, size_t _n, size_t _stride = 1)
        : data(_data), n(_n), stride(_stride) {
      if (!data) {
        throw std::invalid_argument("data is nullptr");
      }
      if (n == 0) {
        throw std::invalid_argument("n is 0");
      }
      if (stride == 0) {
        throw std::invalid_argument("stride is 0");
      }
    }

    element_type &at(size_t index) const { return data[index * stride]; }

    auto operator*=(const element_type &scale) -> auto & {
      for (size_t i = 0; i < n; i++) {
        at(i) *= scale;
      }
      return *this;
    }

    auto operator+=(const vector_view<element_type> &rhs) -> auto & {
      if (!same_dimension(rhs)) {
        throw exception::no_same_set("different dimension");
      }
      for (size_t i = 0; i < n; i++) {
        at(i) += rhs.at(i);
      }
      return *this;
    }

    element_type dot(const vector_view<element_type> &rhs) {
      if (!same_dimension(rhs)) {
        throw exception::no_same_set("different dimension");
      }
      element_type res = 0;
      for (size_t i = 0; i < n; i++) {
        res += at(i) * rhs.at(i);
      }
      return res;
    }

    bool same_dimension(const vector_view<element_type> &rhs) const {
      return n == rhs.n;
    }

  private:
    T *data{nullptr};
    size_t n{0};
    size_t stride{1};
  };
} // namespace cyy::math::la
