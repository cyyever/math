/*!
 * \file matrix.hpp
 *
 * \brief matrix related classes
 */
#pragma once

#include <optional>
#include <range/v3/view.hpp>

#include "vector.hpp"
namespace cyy::math::la {

  template <typename T,
            typename = std::enable_if_t<cyy::math::type::is_numerical_type<T>>>
  class matrix_view {
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
        row_vectors.emplace_back(data, col_num, stride);
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
    void add_rows_with_scale(size_t from_index, const element_type &scalar,
                             size_t to_index) {
      auto &to_vector = row_vectors.at(to_index);
      auto &from_vector = row_vectors.at(from_index);
      for (size_t i = 0; i < col_num; i++) {
        to_vector[i] += from_vector[i] * scalar;
      }
    }

    auto to_matrix() const {
      std::vector<std::vector<T>> result;
      for (auto const &row : row_vectors) {
        result.emplace_back(row.to_vector());
      }
      return result;
    }

  protected:
    size_t row_num;
    size_t col_num;
    std::vector<::cyy::math::la::vector_view<T>> row_vectors;
  };

  template <typename T> class square_matrix_view : public matrix_view<T> {
  public:
    square_matrix_view(T *data, size_t row_num_, size_t stride = 1,
                       size_t row_stride = 0)
        : matrix_view<T>(data, row_num_, row_num_, stride, row_stride) {}

    T determinant() {
      T result = 1;

      for (size_t i = 0; i < this->row_num; i++) {
        if (this->row_vectors[i][i] == 0) {
          auto it = std::find_if(this->row_vectors.begin() + i,
                                 this->row_vectors.end(),
                                 [i](auto const &e) { return e.at(i) != 0; });
          if (it == this->row_vectors.end()) {
            return 0;
          }

          auto j = std::distance(this->row_vectors.begin(), it);
          if ((j - i) % 2 == 1) {
            result = -result;
          }
          this->swap_row(i, j);
        }
        const auto &pivot = this->row_vectors[i].at(i);

        result *= pivot;
        for (size_t j = i + 1; j < this->row_num; j++) {
          if (this->row_vectors[j][i] == 0) {
            continue;
          }

          this->add_rows_with_scale(i, -this->row_vectors[j][i] / pivot, j);
        }
      }

      return result;
    }

    std::optional<std::vector<std::vector<T>>> invert() const {
      auto result = this->to_matrix();

      for (auto const &row : this->row_vectors) {
        result.emplace_back(row);
      }

      for (size_t i = 0; i < this->row_num; i++) {
        result[i].insert(result[i].end(), i, 0);
        result[i].push_back(1);
        result[i].insert(result[i].end(), this->row_num - i - 1, 0);
      }
      for (size_t i = 0; i < this->row_num; i++) {
        if (result[i][i] == 0) {
          auto it = std::find_if(result.begin() + i, result.end(),
                                 [i](auto const &e) { return e.at(i) != 0; });
          if (it == result.end()) {
            return {};
          }

          std::swap(result[i], *it);
        }
        auto &pivot = result[i][i];
        if (pivot != 1) {
          vector_view(result[i]) *= 1 / pivot;
        }

        for (size_t j = 0; j < this->row_num; j++) {
          if (j == i || result[j][i] == 0) {
            continue;
          }
          for (size_t k = 0; k < this->row_num * 2; k++) {
            result[j][k] -= result[i][k] * pivot;
          }
        }
      }
      return result;
    }
  };

} // namespace cyy::math::la
