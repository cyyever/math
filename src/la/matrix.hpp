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

    matrix_view(element_type *data, size_t rows, size_t cols, size_t stride = 1,
                size_t row_stride = 0) {
      if (!data) {
        throw std::invalid_argument("data is nullptr");
      }
      if (rows == 0) {
        throw std::invalid_argument("rows is 0");
      }
      if (cols == 0) {
        throw std::invalid_argument("cols is 0");
      }
      if (row_stride == 0) {
        row_stride = cols;
      }
      if (row_stride < cols) {
        throw std::invalid_argument("row_stride<cols");
      }
      row_vectors.reserve(rows);
      for (size_t i = 0; i < rows; i++) {
        row_vectors.emplace_back(data, rows, stride);
        data += row_stride;
      }
    }

  private:
    std::vector<::cyy::math::la::vector_view<T>> row_vectors;
  };
} // namespace cyy::math::la
