/*!
 * \file vector.hpp
 *
 * \brief vector related classes
 */
#pragma once

#include <sstream>
#include <vector>

#include "../exception.hpp"
#include "../numerical_type.hpp"

namespace cyy::math::la {

  template <typename T,
            typename = std::enable_if_t<cyy::math::type::is_numerical_type<T>>>
  class vector_view final {
  public:
    using element_type = T;
    vector_view(T *data_, size_t dimension_, size_t stride_ = 1)
        : data(data_), dimension(dimension_), stride(stride_) {
      if (!data) {
        throw std::invalid_argument("data is nullptr");
      }
      if (dimension == 0) {
        throw std::invalid_argument("dimension is 0");
      }
      if (stride == 0) {
        throw std::invalid_argument("stride is 0");
      }
    }

    vector_view(::std::vector<element_type> &vect)
        : vector_view(vect.data(), vect.size()) {}
    vector_view(const vector_view &) = default;
    vector_view &operator=(const vector_view &) = default;

    vector_view(vector_view &&) noexcept = default;
    vector_view &operator=(vector_view &&) noexcept = default;
    ~vector_view() noexcept = default;
    element_type &at(size_t index) const { return data[index * stride]; }
    element_type &operator[](size_t index) const {
      return data[index * stride];
    }

    bool operator==(const vector_view<element_type> &rhs) const {
      if (!same_dimension(rhs)) {
        return false;
      }
      for (size_t i = 0; i < dimension; i++) {

        if (operator[](i) != rhs.operator[](i)) {
          return false;
        }
      }

      return true;
    }
    bool operator!=(const vector_view<element_type> &rhs) const {
      return !operator==(rhs);
    }
    auto operator*=(const element_type &scalar) -> auto & {
      for (size_t i = 0; i < dimension; i++) {
        operator[](i) *= scalar;
      }
      return *this;
    }

    auto operator+=(const vector_view<element_type> &rhs) -> auto & {
      if (!same_dimension(rhs)) {
        throw exception::no_same_set("different dimension");
      }
      for (size_t i = 0; i < dimension; i++) {
        operator[](i) += rhs.operator[](i);
      }
      return *this;
    }

    auto scaled_sum(const vector_view<element_type> &rhs,
                    const element_type &scalar) -> auto & {
      if (!same_dimension(rhs)) {
        throw exception::no_same_set("different dimension");
      }
      for (size_t i = 0; i < dimension; i++) {
        operator[](i) += rhs.operator[](i) * scalar;
      }
      return *this;
    }

    element_type dot(const vector_view<element_type> &rhs) {
      if (!same_dimension(rhs)) {
        throw exception::no_same_set("different dimension");
      }
      element_type res = 0;
      for (size_t i = 0; i < dimension; i++) {
        res += operator[](i) * rhs.operator[](i);
      }
      return res;
    }

    bool same_dimension(const vector_view<element_type> &rhs) const {
      return dimension == rhs.dimension;
    }

    size_t get_dimension() const { return dimension; }

    std::vector<element_type> to_vector() const {
      std::vector<element_type> result(dimension);
      for (size_t i = 0; i < dimension; i++) {
        result[i] = operator[](i);
      }
      return result;
    }

    std::string to_string() const {
      std::stringstream s;
      for (size_t i = 0; i < dimension; i++) {
        s << operator[](i);
        s << ' ';
      }
      return s.str();
    }

  private:
    T *data{nullptr};
    size_t dimension{0};
    size_t stride{1};
  };
} // namespace cyy::math::la
