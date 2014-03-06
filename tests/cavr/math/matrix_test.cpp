#include <cavr/math/matrix.h>
#include "gtest/gtest.h"
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
