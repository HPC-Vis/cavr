//#include <cavr/math/vector/vector.h>
#include <cavr/math/vector.h>
#include "gtest/gtest.h"
using cavr::math::vector::vec;
using cavr::math::autovec;

TEST(vector_size, vector_size) {
  EXPECT_EQ(sizeof(int) * 3, sizeof(vec<int, 3>));
}

TEST(vector_construction, default_constructor) {
  vec<int, 3> a;
  vec<float, 2> b;
  vec<char, 4> c;
}

TEST(vector_equality, vector_equality) {
  vec<int, 3> a(1, 2, 3);
  vec<char, 3> b(1, 2, 3);
  vec<int, 3> c(2, 3, 1);
  EXPECT_EQ(autovec(1.0, 2.0, 3.0), a);
  EXPECT_EQ(b, a);
  EXPECT_EQ(a, a);
  EXPECT_EQ(a, c.zxy);
}

TEST(vector_construction, single_value_constructor) {
  vec<int, 4> v(7);
  EXPECT_EQ(autovec(7,7,7,7), v);

  vec<int, 3> w = vec<int, 3>(8);
  EXPECT_EQ(autovec(8,8,8), w);

  vec<int, 7> a = vec<int, 7>(1);
  EXPECT_EQ(autovec(1, 1, 1, 1, 1, 1, 1), a);
}

TEST(vector_construction, n_value_constructor) {
  char c = 1;
  vec<int, 4> v(3, 2.0, c, 0 * c);
  EXPECT_EQ(autovec(3, 2, 1, 0), v);
}

TEST(vector_construction, vec_constructor) {
  vec<float, 4> v(3, 2, 1, 0);
  vec<int, 4> w(v);
  EXPECT_EQ(autovec(3, 2, 1, 0), w);

  vec<int, 2> x(9, 8);
  vec<int, 2> y(7, 6);
  vec<int, 4> z(x, y);
  EXPECT_EQ(autovec(9, 8, 7, 6), z);

  vec<int, 3> a(1, x);
  EXPECT_EQ(autovec(1, 9, 8), a);

  vec<int, 3> b(y, 5);
  EXPECT_EQ(autovec(7, 6, 5), b);
}

TEST(vector_construction, swizzle_construction) {
  vec<int, 2> v12(1, 2);
  vec<int, 2> v34(3, 4);
  vec<int, 4> a(v12.xx, v12.yy);
  EXPECT_EQ(autovec(1, 1, 2, 2), a);
  EXPECT_EQ(autovec(1, 2, 1, 1), (vec<int, 4>(v12, v12.xx)));

  vec<int, 4> b(v12.xy, v34.xy);
  EXPECT_EQ(autovec(1, 2, 3, 4), b);

  vec<int, 3> v567(5, 6, 7);
  vec<int, 3> c(v567.zyx);
  EXPECT_EQ(autovec(7, 6, 5), c);

  vec<int, 3> d(v12.yyy);
  EXPECT_EQ(autovec(2, 2, 2), d);

  vec<int, 4> e(0, v12.xxx);
  EXPECT_EQ(autovec(0, 1, 1, 1), e);
}

TEST(vector_assignment, vector_assignment) {
  vec<int, 4> a(1, 2, 3, 4);
  vec<int, 4> b = a;
  EXPECT_EQ(autovec(1, 2, 3, 4), b);
  vec<float, 4> c(5, 6, 7, 8);
  b = c;
  EXPECT_EQ(autovec(5, 6, 7, 8), b);
  c = (b = a);
  EXPECT_EQ(autovec(1, 2, 3, 4), c);
}

TEST(vector_assignment, swizzle_assignment) {
  vec<int, 2> a(1, 2);
  vec<int, 4> b = a.rrrr;
  EXPECT_EQ(autovec(1, 1, 1, 1), b);
  b = a.yxyx;
  EXPECT_EQ(autovec(2, 1, 2, 1), b);
  a = a.yx;
  EXPECT_EQ(autovec(2, 1), a);
  vec<int, 4> c(1, 2, 3, 4);
  b.xzyw = c;
  EXPECT_EQ(autovec(1, 3, 2, 4), b);
  c.wzyx = c.yyxx; // 2 2 1 1
  EXPECT_EQ(autovec(1, 1, 2, 2), c);
  c.xyz = vec<int, 3>(3);
  EXPECT_EQ(autovec(3, 3, 3, 2), c);
}

TEST(vector_addition, vector_addition) {
  vec<int, 3> a(1, 2, 3);
  vec<int, 3> b(4, 5, 6);
  vec<int, 3> c = a + b;
  EXPECT_EQ(autovec(5, 7, 9), c);
  c = a + a;
  EXPECT_EQ(autovec(2, 4, 6), c);
  c += a;
  EXPECT_EQ(autovec(3, 6, 9), c);
  c += c;
  EXPECT_EQ(autovec(6, 12, 18), c);
  c += c.zyx;
  EXPECT_EQ(autovec(24, 24, 24), c);
  c = a.xxx + b.zyx;
  EXPECT_EQ(autovec(7, 6, 5), c);
  c.yzx += vec<int, 3>(2, 3, 1);
  EXPECT_EQ(autovec(8, 8, 8), c);
}

TEST(vector_subtraction, vector_subtraction) {
  vec<int, 3> a(1, 2, 3);
  vec<int, 3> b(4, 5, 6);
  vec<int, 3> c = a - b;
  EXPECT_EQ(autovec(-3, -3, -3), c);
  c = a - a;
  EXPECT_EQ(autovec(0, 0, 0), c);
  c -= a;
  EXPECT_EQ(autovec(-1, -2, -3), c);
  c -= c;
  EXPECT_EQ(autovec(0, 0, 0), c);
  c -= b.xzy;
  EXPECT_EQ(autovec(-4, -6, -5), c);
  c = b.zzz - a;
  EXPECT_EQ(autovec(5, 4, 3), c);
  c.zyx -= a.zyx;
  EXPECT_EQ(autovec(4, 2, 0), c);
}

TEST(vector_negation, vector_negation) {
  vec<int, 3> a(1, 2, 3);
  vec<int, 3> b = -a;
  EXPECT_EQ(autovec(-1, -2, -3), b);
  EXPECT_EQ(autovec(1, 2, 3), a);
  EXPECT_EQ(autovec(-3, -2, -1), -a.zyx);
  EXPECT_EQ(autovec(-3, -2, -1), -(a.zyx));
}

TEST(vector_multiplication, vector_multiplication) {
  vec<int, 3> a(1, 2, 3);
  vec<int, 3> b = a * 4;
  EXPECT_EQ(autovec(4, 8, 12), b);
  EXPECT_EQ(autovec(1, 2, 3), a);
  b *= 10;
  EXPECT_EQ(autovec(40, 80, 120), b);
  EXPECT_EQ(autovec(2, 4, 6), 2 * a);
  EXPECT_EQ(autovec(4, 2, 6), a.yxz * 2);
  EXPECT_EQ(autovec(6, 6, 4), 2 * a.zzy);
}

TEST(vector_division, vector_division) {
  vec<int, 3> a(3, 6, 9);
  vec<int, 3> b = a / 3;
  EXPECT_EQ(autovec(1, 2, 3), b);
  b = a / 2.0;
  EXPECT_EQ(autovec(1, 3, 4), b);
  EXPECT_EQ(autovec(3, 2, 1), a.zyx / 3);
}

TEST(vector_dot_product, vector_dot_product) {
  vec<int, 3> a(1, 2, 3);
  vec<int, 3> b(4, 5, 6);
  int d = a.dot(b);
  EXPECT_EQ(32, d);
  EXPECT_EQ(autovec(1, 2, 3), a);
  EXPECT_EQ(autovec(4, 5, 6), b);
  EXPECT_EQ(18, a.zzz.dot(a));
}

TEST(vector_cross_product, vector_cross_product) {
  vec<int, 3> a(1, 0, 0);
  vec<int, 3> b(0, 1, 0);
  EXPECT_EQ(autovec(0, 0, 1), a.cross(b));
  EXPECT_EQ(autovec(0, 0, -1), b.cross(a));
  a = vec<int, 3>(1, 2, 3);
  EXPECT_EQ(autovec(0, 0, 0), a.cross(a));
  EXPECT_EQ(autovec(1, 2, 3), a);
  EXPECT_EQ(autovec(0, 0, 1), b.yxx.cross(b));
  EXPECT_EQ(autovec(0, 0, -1), b.cross(b.yxz));
}

TEST(vector_normalization, vector_normalization) {
  vec<int, 3> a(3, 4, 0);
  EXPECT_EQ(25, a.length_squared());
  EXPECT_EQ(5, a.length());
  vec<int, 2> b(100, 0);
  EXPECT_EQ(autovec(1, 0), b.normalized());
}
