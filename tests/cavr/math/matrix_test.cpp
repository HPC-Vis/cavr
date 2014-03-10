#include <cavr/math/matrix.h>
#include <cavr/math/vector.h>
#include "gtest/gtest.h"

#include "check_vector.h"
using cavr::math::matrix::mat;
using cavr::math::vector::vec;
using cavr::math::autovec;

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
  EXPECT_EQ(autovec(0, 1, 2), b[0]);
  EXPECT_EQ(autovec(3, 4, 5), b[1]);
}

TEST(matrix_access, row_access) {
  vec<int, 3> a(0, 1, 2);
  vec<int, 4> b(3, 4, 5, 6);
  mat<int, 3, 3> m(a, b, 7, 8);
  EXPECT_EQ(autovec(0, 3, 6), m.row(0));
  EXPECT_EQ(autovec(1, 4, 7), m.row(1));
  EXPECT_EQ(autovec(2, 5, 8), m.row(2));
}

TEST(matrix_access, column_access) {
  vec<int, 3> a(0, 1, 2);
  vec<int, 4> b(3, 4, 5, 6);
  mat<int, 3, 3> m(a, b, 7, 8);
  EXPECT_EQ(autovec(0, 1, 2), m[0]);
  EXPECT_EQ(autovec(3, 4, 5), m[1]);
  EXPECT_EQ(autovec(6, 7, 8), m[2]);
}

TEST(matrix_transpose, matrix_transpose) {
  mat<int, 2, 4> m(0, 1, 2, 3, 4, 5, 6, 7);
  EXPECT_EQ(autovec(0, 1, 2, 3), m[0]);
  EXPECT_EQ(autovec(4, 5, 6, 7), m[1]);
  mat<int, 4, 2> t = m.transpose();
  EXPECT_EQ(autovec(0, 1, 2, 3), t.row(0));
  EXPECT_EQ(autovec(4, 5, 6, 7), t.row(1));
}

TEST(matrix_multiplication, matrix_multiplication) {
  mat<int, 2, 3> a(0, 1, 2, 3, 4, 5);
  mat<int, 4, 2> b(1, 2, 2, 0, 2, 0, 1, 3);
  mat<int, 4, 3> c = a * b;
  EXPECT_EQ(autovec(6, 0, 0, 9), c.row(0));
  EXPECT_EQ(autovec(9, 2, 2, 13), c.row(1));
  EXPECT_EQ(autovec(12, 4, 4, 17), c.row(2));
}

TEST(matrix_multiplication, vector_multiplication) {
  mat<int, 3, 2> a(0, 3, 1, 4, 2, 5);
  vec<int, 3> b(1, 2, 3);
  vec<int, 2> c = a * b;
  EXPECT_EQ(autovec(8, 26), c);
}

TEST(matrix_multiplication, square_multiplication) {
  mat<int, 2, 2> a(0, 1, 2, 3);
  mat<int, 2, 2> b(4, 5, 6, 7);
  mat<int, 2, 2> c = a * b;
  EXPECT_EQ(autovec(10, 14), c.row(0));
  EXPECT_EQ(autovec(19, 27), c.row(1));
  c = a;
  c *= a;
  EXPECT_EQ(autovec(2,  6), c.row(0));
  EXPECT_EQ(autovec(3, 11), c.row(1));
  a *= a;
  EXPECT_EQ(autovec(2,  6), a.row(0));
  EXPECT_EQ(autovec(3, 11), a.row(1));
}

TEST(matrix_multiplication, scalar_multiplication) {
  mat<int, 2, 3> a(0, 1, 2, 3, 4, 5);
  mat<int, 2, 3> b = a * 2;
  EXPECT_EQ(autovec(0, 2,  4), b[0]);
  EXPECT_EQ(autovec(6, 8, 10), b[1]);
  mat<int, 2, 3> c = a;
  c *= 3;
  EXPECT_EQ(autovec(0,  3,  6), c[0]);
  EXPECT_EQ(autovec(9, 12, 15), c[1]);
  c = 2 * a;
  EXPECT_EQ(autovec(0, 2,  4), c[0]);
  EXPECT_EQ(autovec(6, 8, 10), c[1]);
}

TEST(matrix_division, scalar_division) {
  mat<int, 2, 3> a(0, 2, 4, 6, 8, 10);
  mat<int, 2, 3> b = a / 2;
  EXPECT_EQ(autovec(0, 1,  2), b[0]);
  EXPECT_EQ(autovec(3, 4,  5), b[1]);
  EXPECT_EQ(autovec(0, 2,  4), a[0]);
  EXPECT_EQ(autovec(6, 8, 10), a[1]);
  a /= 2;
  EXPECT_EQ(autovec(0, 1, 2), a[0]);
  EXPECT_EQ(autovec(3, 4, 5), a[1]);
}

TEST(homogeneous_matrix, translation) {
  mat<int, 4, 4> a = mat<int, 4, 4>::translate(4, 3, 2);
  EXPECT_EQ(autovec(1, 0, 0, 4), a.row(0));
  EXPECT_EQ(autovec(0, 1, 0, 3), a.row(1));
  EXPECT_EQ(autovec(0, 0, 1, 2), a.row(2));
  EXPECT_EQ(autovec(0, 0, 0, 1), a.row(3));
  mat<int, 4, 4> b = mat<int, 4, 4>::translate(vec<int, 3>(5, 6, 7));
  EXPECT_EQ(autovec(1, 0, 0, 5), b.row(0));
  EXPECT_EQ(autovec(0, 1, 0, 6), b.row(1));
  EXPECT_EQ(autovec(0, 0, 1, 7), b.row(2));
  EXPECT_EQ(autovec(0, 0, 0, 1), b.row(3));
}

TEST(homogeneous_matrix, rotation) {
  mat<int, 4, 4> a = mat<int, 4, 4>::rotate(M_PI / 2, 0, 0, 1);
  EXPECT_EQ(autovec(0, -1,  0, 0), a.row(0));
  EXPECT_EQ(autovec(1,  0,  0, 0), a.row(1));
  EXPECT_EQ(autovec(0,  0,  1, 0), a.row(2));
  EXPECT_EQ(autovec(0,  0,  0, 1), a.row(3));
  mat<int, 4, 4> b = mat<int, 4, 4>::rotate(M_PI / 2, vec<int, 3>(1, 0, 0));
  EXPECT_EQ(autovec(1,  0,  0, 0), b.row(0));
  EXPECT_EQ(autovec(0,  0, -1, 0), b.row(1));
  EXPECT_EQ(autovec(0,  1,  0, 0), b.row(2));
  EXPECT_EQ(autovec(0,  0,  0, 1), b.row(3));
}

TEST(homogeneous_matrix, look_at) {
  mat<int, 4, 4> a = mat<int, 4, 4>::look_at(autovec(1,0,0),
                                             autovec(0,0,0),
                                             autovec(0,1,0));
  EXPECT_EQ(autovec(0, 0, -1, 1), a * autovec(0, 0, 0, 1));
  EXPECT_EQ(autovec(0, 0, -2, 1), a * autovec(-1, 0, 0, 1));
  EXPECT_EQ(autovec(0, 0, 0, 1), a * autovec(1, 0, 0, 1));
  EXPECT_EQ(autovec(1, 1, -1, 1), a * autovec(0, 1, -1, 1));
}
