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

    matrix_view(element_type *data_, size_t row_num_, size_t col_num_,
                size_t stride_ = 1, size_t row_stride_ = 0)
        : data(data_), row_num(row_num_), col_num(col_num_), stride(stride_),
          row_stride(row_stride_) {
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
      row_vectors.reserve(row_num);
      for (size_t i = 0; i < row_num; i++) {
        row_vectors.emplace_back(data_, col_num, stride);
        data_ += row_stride;
      }
    }

    matrix_view(const matrix_view &) = default;
    matrix_view &operator=(const matrix_view &) = default;

    matrix_view(matrix_view &&) noexcept = default;
    matrix_view &operator=(matrix_view &&) noexcept = default;
    ~matrix_view() noexcept = default;

    matrix_view transpose() const {
      return matrix_view(data, col_num, row_num, row_stride, stride);
    }

    bool same_dimension(const matrix_view<element_type> &rhs) const {
      return row_num == rhs.row_num && col_num == rhs.col_num;
    }
    bool operator==(const matrix_view<element_type> &rhs) const {
      if (!same_dimension(rhs)) {
        return false;
      }

      for (size_t i = 0; i < row_num; i++) {
        if (row_vectors[i] != rhs.row_vectors[i]) {
          return false;
        }
      }

      return true;
    }
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
      row_vectors.at(to_index).scaled_sum(row_vectors.at(from_index), scalar);
    }

    auto to_matrix() const {
      std::vector<std::vector<T>> result;
      for (auto const &row : row_vectors) {
        result.emplace_back(row.to_vector());
      }
      return result;
    }
    std::string to_string() const {
      std::stringstream s;
      for (auto const &v : row_vectors) {
        s << v.to_string();
        s << std::endl;
      }
      return s.str();
    }

  protected:
    element_type *data{};
    size_t row_num;
    size_t col_num;
    size_t stride;
    size_t row_stride;
    std::vector<::cyy::math::la::vector_view<T>> row_vectors;
  };

  template <typename T> class square_matrix_view : public matrix_view<T> {
  public:
    square_matrix_view(T *data_, size_t row_num_, size_t stride_ = 1,
                       size_t row_stride_ = 0)
        : matrix_view<T>(data_, row_num_, row_num_, stride_, row_stride_) {}

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
      for (size_t i = 0; i < this->row_num; i++) {
        result[i].insert(result[i].end(), i, 0);
        result[i].push_back(1);
        result[i].insert(result[i].end(), this->row_num - 1 - i, 0);
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
          vector_view(result[j]).scaled_sum(vector_view(result[i]),
                                            -result[j][i]);
        }
      }
      for (auto &vec : result) {
        vec.erase(vec.begin(), vec.begin() + this->row_num);
      }
      return result;
    }
  };

  template <typename T,
            typename = std::enable_if_t<cyy::math::type::is_numerical_type<T>>>
  std::vector<std::vector<T>> get_identity_matrix(size_t N) {

    std::vector<std::vector<T>> result;
    for (size_t i = 0; i < N; i++) {
      result[i].resize(N, 0);
      result[i][i] = 1;
    }
    return result;
  }

} // namespace cyy::math::la
