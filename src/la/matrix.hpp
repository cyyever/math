/*!
 * \file matrix.hpp
 *
 * \brief matrix related classes
 */
#pragma once

#include <range/v3/view.hpp>

#include "vector.hpp"
namespace cyy::math::la {

  template <typename T,
            typename = std::enable_if_t<cyy::math::type::is_numerical_type<T>>>
  class matrix_view final {
  public:
    using element_type = T;

    matrix_view(element_type *data, size_t row_num_, size_t col_num_,
                size_t stride = 1, size_t row_stride = 0)
        : row_num(row_num_), col_num(col_num_) {
      if (!data) {
        throw std::invalid_argument("data is nullptr");
      }
      if (row_num == 0) {
        throw std::invalid_argument("row_num is 0");
      }
      if (col_num == 0) {
        throw std::invalid_argument("col_num is 0");
      }
      if (row_stride == 0) {
        row_stride = col_num;
      }
      if (row_stride < col_num) {
        throw std::invalid_argument("row_stride<col_num");
      }
      row_vectors.reserve(row_num);
      for (size_t i = 0; i < row_num; i++) {
        row_vectors.emplace_back(data, row_num, stride);
        data += row_stride;
      }
    }

    matrix_view(const matrix_view &) = default;
    matrix_view &operator=(const matrix_view &) = default;

    matrix_view(matrix_view &&) noexcept = default;
    matrix_view &operator=(matrix_view &&) noexcept = default;
    ~matrix_view() noexcept = default;

    void swap_row(size_t i, size_t j) {
      std::swap(row_vectors.at(i), row_vectors.at(j));
    }
    void multiply_row(size_t i, const element_type &scalar) {
      row_vectors.at(i) *= scalar;
    }

    void add_rows(size_t from_index, size_t to_index) {
      row_vectors.at(to_index) += row_vectors.at(from_index);
    }

    bool is_invertible() const { return row_num == col_num; }

  private:
    size_t row_num;
    size_t col_num;
    std::vector<::cyy::math::la::vector_view<T>> row_vectors;
  };
} // namespace cyy::math::la
