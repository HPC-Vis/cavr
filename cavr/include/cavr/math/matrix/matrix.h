#pragma once
#include <algorithm>
#include <iostream>

#include <cavr/math/vector/traits.h>
#include <cavr/math/vector/vector.h>

namespace cavr {

namespace math {

namespace matrix {

template<typename T, int N, typename M, bool S = false>
struct square_matrix {
};

template<typename T, int N, typename M>
struct square_matrix<T, N, M, true> {
  inline M inverted(bool* success = nullptr) const {
    // Since we can access columns easily, we invert on the transpose
    M inverse(1.0);
    M m = static_cast<const M*>(this)->transposed();
    for (int c = 0; c < N; ++c) {
      int pivot_column = c;
      T pivot_value = m[pivot_column][c];
      for (int d = c + 1; d < N; ++d) {
        T d_value = m[d][c];
        if (std::fabs(pivot_value) < std::fabs(d_value)) {
          pivot_column = d;
          pivot_value = d_value;
        }
      }
      if (c != pivot_column) {
        std::swap(m[c], m[pivot_column]);
        std::swap(inverse[c], inverse[pivot_column]);
      }
      if (pivot_value == 0) {
        if (success) {
          *success = false;
        }
        return inverse;
      }
      T coefficient = T(1) / pivot_value;
      m[c] *= coefficient;
      inverse[c] *= coefficient;
      for (int d = 0; d < N; ++d) {
        if (d == c) {
          continue;
        }
        T mults = m[d][c];
        m[d] -= mults * m[c];
        inverse[d] -= mults * inverse[c];
      }
    }
    if (success) {
      *success = true;
    }
    return inverse.transposed();
  };
};

template<typename T, typename M, bool H = false>
struct homogeneous_matrix {
};

template<typename T, typename M>
struct homogeneous_matrix<T, M, true> {
  template<typename... U,
           typename =
             typename std::enable_if<vector::dims<U...>::value == 3>::type>
  static inline M translate(U&&... u) {
    M result(1);
    result[3].xyz = vector::vec<T, 3>(std::forward<U>(u)...);
    return result;
  }

  template<typename R,
           typename... U,
           typename =
             typename std::enable_if<vector::dims<U...>::value == 3>::type>
  static inline M rotate(const R& radians, U&&... u) {
    M result(1);
    vector::vec<T, 3> axis(std::forward<U>(u)...);
    double cos_r = std::cos(radians);
    double sin_r = std::sin(radians);
    double one_minus_cos_r = 1.0 - cos_r;
    double xx = axis.x * axis.x;
    double xy = axis.x * axis.y;
    double xz = axis.x * axis.z;
    double yy = axis.y * axis.y;
    double yz = axis.y * axis.z;
    double zz = axis.z * axis.z;
    double x_sin_r = axis.x * sin_r;
    double y_sin_r = axis.y * sin_r;
    double z_sin_r = axis.z * sin_r;
    double xy_one_minus_cos_r = xy * one_minus_cos_r;
    double xz_one_minus_cos_r = xz * one_minus_cos_r;
    double yz_one_minus_cos_r = yz * one_minus_cos_r;
    result[0][0] = cos_r + xx * one_minus_cos_r;
    result[0][1] = xy_one_minus_cos_r + z_sin_r;
    result[0][2] = xz_one_minus_cos_r - y_sin_r;
    result[1][0] = xy_one_minus_cos_r - z_sin_r;
    result[1][1] = cos_r + yy * one_minus_cos_r;
    result[1][2] = yz_one_minus_cos_r + x_sin_r;
    result[2][0] = xz_one_minus_cos_r + y_sin_r;
    result[2][1] = yz_one_minus_cos_r - x_sin_r;
    result[2][2] = cos_r + zz * one_minus_cos_r;
    return result;
  }

  template<typename E,
           typename L,
           typename U,
           typename =
             typename std::enable_if<vector::dims<E>::value == 3>::type,
           typename =
             typename std::enable_if<vector::dims<L>::value == 3>::type,
           typename =
             typename std::enable_if<vector::dims<U>::value == 3>::type>
  static inline M look_at(const E& eye_point,
                          const L& look_point,
                          const U& up_direction) {
    vector::vec<double, 3> e(eye_point);
    vector::vec<double, 3> l(look_point);
    vector::vec<double, 3> u(up_direction);
    auto z_axis = (l- e).normalized();
    auto x_axis = z_axis.cross(u.normalized());
    x_axis = x_axis.normalized();
    auto y_axis = x_axis.cross(z_axis).normalized();
    M result(1);
    result[0][0] = x_axis.x;
    result[0][1] = y_axis.x;
    result[0][2] = -z_axis.x;

    result[1][0] = x_axis.y;
    result[1][1] = y_axis.y;
    result[1][2] = -z_axis.y;

    result[2][0] = x_axis.z;
    result[2][1] = y_axis.z;
    result[2][2] = -z_axis.z;

    result[3][0] = -x_axis.dot(eye_point);
    result[3][1] = -y_axis.dot(eye_point);
    result[3][2] = z_axis.dot(eye_point);

    return result;
  }

  static inline M ortho(double left,
                        double right,
                        double bottom,
                        double top,
                        double near,
                        double far) {
    double dx = right - left;
    double dy = top - bottom;
    double dz = far - near;
    double tx = -(right + left) / dx;
    double ty = -(top + bottom) / dy;
    double tz = -(far + near) / dz;
    M result(2.0 / dx, 0, 0, 0, // col 0
             0, 2.0 / dy, 0, 0, // col 1
             0, 0, -2.0 / dz, 0, // col 2,
             tx, ty, tz, 1); // col 3
    return result;
  }

  static inline M perspective(double field_of_view_y_radians,
                              double aspect_ratio,
                              double near,
                              double far) {
    double f = 1.0 / std::tan(field_of_view_y_radians * 0.5);
    double dz = near - far;
    M result(f / aspect_ratio, 0, 0, 0, // col 0,
             0, f, 0, 0, // col 1,
             0, 0, (far + near) / dz, -1, // col 2,
             0, 0, 2.0 * far * near / dz, 0); // col 3
    return result;
  }
};

template<typename T, int C, int R>
struct mat 
  : public homogeneous_matrix<T, mat<T, C, R>, (C == 4) && (R == 4)>,
    public square_matrix<T, C, mat<T, C, R>, C == R> {
  union {
    T v[R * C];
    struct {
      vector::vec<T, R> columns[C];
    };
  };

  inline const vector::vec<T, R>& operator[](int i) const {
    return columns[i];
  }

  inline vector::vec<T, R>& operator[](int i) {
    return columns[i];
  }

  inline mat() {
  }

  template<typename U,
           typename =
             typename std::enable_if<std::is_convertible<U, T>::value>::type>
  inline mat(const U& u) {
    zero_();
    mat& self = *this;
    for (int i = 0; i < std::min(C, R); ++i) {
      self[i][i] = u;
    }
  }

  template<typename... U,
           typename =
             typename std::enable_if<vector::dims<U...>::value == R * C>::type>
  inline mat(U&&... u) {
    vector::assign(v, std::forward<U>(u)...);
  }

  template<typename U, int CU, int RU>
  inline mat(const mat<U, CU, RU>& m) {
    zero_();
    mat& self = *this;
    for (int i = 0; i < std::min(C, R); ++i) {
      self[i][i] = 1;
    }
    for (int c = 0; c < std::min(C, CU); ++c) {
      for (int r = 0; r < std::min(R, RU); ++r) {
        self[c][r] = m[c][r];
      }
    }
  }

  inline vector::vec<T, C> row(int i) const {
    vector::vec<T, C> result;
    for (int c = 0; c < C; ++c) {
      result[c] = columns[c][i];
    }
    return result;
  }

  inline mat<T, R, C> transposed() const {
    mat<T, R, C> result;
    for (int r = 0; r < R; ++r) {
      result[r] = row(r);
    }
    return result;
  }

  template<typename U>
  inline mat& operator=(const mat<U, C, R>& rhs) {
    for (int i = 0; i < C * R; ++i) {
      v[i] = rhs.v[i];
    }
  }

  template<typename U, int CU>
  inline mat<typename std::common_type<T, U>::type, CU, R>
  operator*(const mat<U, CU, C>& u) const {
    mat<typename std::common_type<T, U>::type, CU, R> result;
    for (int r = 0; r < R; ++r) {
      const auto row_vector = row(r);
      for (int c = 0; c < CU; ++c) {
        const auto& column_vector = u[c];
        result[c][r] = row_vector.dot(column_vector);
      }
    }
    return result;
  }

  template<typename U>
  inline mat<typename std::common_type<T, U>::type, C, R>
  operator*(const U& u) const {
    mat<typename std::common_type<T, U>::type, C, R> result(*this);
    for (int i = 0; i < C * R; ++i) {
      result.v[i] *= u;
    }
    return result;
  }

  template<typename U,
           typename =
             typename std::enable_if<std::is_convertible<U, T>::value>::type>
  inline mat& operator*=(const U& u) {
    for (int i = 0; i < C * R; ++i) {
      v[i] *= u;
    }
    return *this;
  }

  template<typename U>
  struct is_square {
    static const bool value = (C == R);
  };

  template<typename U,
           typename =
             typename std::enable_if<is_square<U>::value>::type>
  inline mat& operator*=(const mat<U, C, R>& m) {
    *this = *this * m;
    return *this;
  }

  template<typename U,
           typename =
             typename std::enable_if<std::is_convertible<U, T>::value>::type>
  friend inline mat<typename std::common_type<T, U>::type, C, R>
  operator*(const U& u, const mat& r) {
    return r * u;
  }

  template<typename U>
  inline mat<typename std::common_type<T, U>::type, C, R>
  operator/(const U& u) const {
    mat<typename std::common_type<T, U>::type, C, R> result(*this);
    for (int i = 0; i < C * R; ++i) {
      result.v[i] /= u;
    }
    return result;
  }

  template<typename U,
           typename =
             typename std::enable_if<std::is_convertible<U, T>::value>::type>
  inline mat& operator/=(const U& u) {
    for (int i = 0; i < C * R; ++i) {
      v[i] /= u;
    }
    return *this;
  }

  template<typename U,
           typename =
             typename std::enable_if<vector::dims<U>::value == C>::type>
  inline vector::vec<typename std::common_type<T, typename U::type>::type, R>
  operator *(const U& u) {
    vector::vec<typename std::common_type<T, typename U::type>::type, R> result;
    for (int r = 0; r < R; ++r) {
      result[r] = row(r).dot(u);
    }
    return result;
  }

private:
  inline void zero_() {
    for (int i = 0; i < R * C; ++i) {
      v[i] = 0;
    }
  }
};

} // namespace matrix

} // namespace math

} // namespace cavr
