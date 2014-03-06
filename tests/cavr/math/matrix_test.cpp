#include <cavr/math/matrix.h>
#include "gtest/gtest.h"

#include "check_vector.h"
using cavr::math::matrix::mat;
using cavr::math::vector::vec;

TEST(matrix_construction, diagonal_construction) {
  mat<int, 3, 4> a(5);
  for (int c = 0; c < 3; ++c) {
    for (int r = 0; r < 4; ++r) {
      if (c == r) {
        EXPECT_EQ(5, a[c][r]);
      } else {
        EXPECT_EQ(0, a[c][r]);
      }
    }
  }
}

TEST(matrix_construction, vector_construction) {
  vec<int, 3> a(0, 1, 2);
  vec<int, 4> b(3, 4, 5, 6);
  mat<int, 3, 3> m(a, b, 7, 8);
  for (int i = 0; i < 9; ++i) {
    EXPECT_EQ(i, m.v[i]);
  }
}

TEST(matrix_construction, submatrix_construction) {
  vec<int, 3> a(0, 1, 2);
  vec<int, 4> b(3, 4, 5, 6);
  mat<int, 3, 3> m(a, b, 7, 8);
  mat<int, 2, 2> n(m);
  EXPECT_EQ(0, n[0][0]);
  EXPECT_EQ(1, n[0][1]);
  EXPECT_EQ(3, n[1][0]);
  EXPECT_EQ(4, n[1][1]);
  mat<int, 3, 3> o(n);
  EXPECT_EQ(0, o[0][0]);
  EXPECT_EQ(1, o[0][1]);
  EXPECT_EQ(0, o[0][2]);
  EXPECT_EQ(3, o[1][0]);
  EXPECT_EQ(4, o[1][1]);
  EXPECT_EQ(0, o[1][2]);
  EXPECT_EQ(0, o[2][0]);
  EXPECT_EQ(0, o[2][1]);
  EXPECT_EQ(1, o[2][2]);
}

TEST(matrix_assignment, matrix_assignment) {
  mat<float, 2, 3> a(0, 1, 2, 3, 4, 5);
  mat<int, 2, 3> b = a;
  check_vector(b[0], 0, 1, 2);
  check_vector(b[1], 3, 4, 5);
}

TEST(matrix_access, row_access) {
  vec<int, 3> a(0, 1, 2);
  vec<int, 4> b(3, 4, 5, 6);
  mat<int, 3, 3> m(a, b, 7, 8);
  check_vector(m.row(0), 0, 3, 6);
  check_vector(m.row(1), 1, 4, 7);
  check_vector(m.row(2), 2, 5, 8);
}

TEST(matrix_access, column_access) {
  vec<int, 3> a(0, 1, 2);
  vec<int, 4> b(3, 4, 5, 6);
  mat<int, 3, 3> m(a, b, 7, 8);
  check_vector(m[0], 0, 1, 2);
  check_vector(m[1], 3, 4, 5);
  check_vector(m[2], 6, 7, 8);
}

TEST(matrix_transpose, matrix_transpose) {
  mat<int, 2, 4> m(0, 1, 2, 3, 4, 5, 6, 7);
  check_vector(m[0], 0, 1, 2, 3);
  check_vector(m[1], 4, 5, 6, 7);
  mat<int, 4, 2> t = m.transpose();
  check_vector(t.row(0), 0, 1, 2, 3);
  check_vector(t.row(1), 4, 5, 6, 7);
}
