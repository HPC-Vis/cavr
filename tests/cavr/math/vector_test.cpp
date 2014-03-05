#include <cavr/math/vector.h>
#include "gtest/gtest.h"
using cavr::math::vec;

template<typename T, typename U>
void check(const T& v,  const U& a) {
  EXPECT_EQ(a, v[0]);
}

template<typename T, typename U>
void check(const T& v,  const U& a, const U& b) {
  EXPECT_EQ(b, v[1]);
  check(v, a);
}

template<typename T, typename U>
void check(const T& v,  const U& a, const U& b, const U& c) {
  EXPECT_EQ(c, v[2]);
  check(v, a, b);
}

template<typename T, typename U>
void check(const T& v,  const U& a, const U& b, const U& c, const U& d) {
  EXPECT_EQ(d, v[3]);
  check(v, a, b, c);
}

TEST(vector_construction, default_constructor) {
  vec<int, 3> a;
  vec<float, 2> b;
  vec<char, 4> c;
}

TEST(vector_construction, single_value_constructor) {
  vec<int, 4> v(7);
  check(v, 7, 7, 7, 7);

  vec<int, 3> w = vec<int, 3>(8);
  check(w, 8, 8, 8);
}

TEST(vector_construction, n_value_constructor) {
  char c = 1;
  vec<int, 4> v(3, 2.0, c, 0 * c);
  check(v, 3, 2, 1, 0);
}

TEST(vector_construction, vec_constructor) {
  vec<float, 4> v(3, 2, 1, 0);
  vec<int, 4> w(v);
  check(w, 3, 2, 1);

  vec<int, 2> x(9, 8);
  vec<int, 2> y(7, 6);
  vec<int, 4> z(x, y);
  check(z, 9, 8, 7, 6);

  vec<int, 3> a(1, x);
  check(a, 1, 9, 8);

  vec<int, 3> b(y, 5);
  check(b, 7, 6, 5);
}

TEST(vector_construction, swizzle_construction) {
  vec<int, 2> v12(1, 2);
  vec<int, 2> v34(3, 4);
  vec<int, 4> a(v12.xx, v12.yy);
  check(a, 1, 1, 2, 2);
  check(vec<int, 4>(v12, v12.xx), 1, 2, 1, 1);

  vec<int, 4> b(v12.xy, v34.xy);
  check(b, 1, 2, 3, 4);

  vec<int, 3> v567(5, 6, 7);
  vec<int, 3> c(v567.zyx);
  check(c, 7, 6, 5);

  vec<int, 3> d(v12.yyy);
  check(d, 2, 2, 2);

  vec<int, 4> e(0, v12.xxx);
  check(e, 0, 1, 1, 1);
}

TEST(vector_assignment, vector_assignment) {
  vec<int, 4> a(1, 2, 3, 4);
  vec<int, 4> b = a;
  check(b, 1, 2, 3, 4);
  vec<float, 4> c(5, 6, 7, 8);
  b = c;
  check(b, 5, 6, 7, 8);
  c = (b = a);
  check(c, 1, 2, 3, 4);
}

TEST(vector_assignment, swizzle_assignment) {
  vec<int, 2> a(1, 2);
  vec<int, 4> b = a.rrrr;
  check(b, 1, 1, 1, 1);
  b = a.yxyx;
  check(b, 2, 1, 2, 1);
  a = a.yx;
  check(a, 2, 1);
  vec<int, 4> c(1, 2, 3, 4);
  b.xzyw = c;
  check(b, 1, 3, 2, 4);
  c.wzyx = c.yyxx; // 2 2 1 1
  check(c, 1, 1, 2, 2);
}

TEST(vector_addition, vector_addition) {
  vec<int, 3> a(1, 2, 3);
  vec<int, 3> b(4, 5, 6);
  vec<int, 3> c = a + b;
  check(c, 5, 7, 9);
  c = a + a;
  check(c, 2, 4, 6);
  c += a;
  check(c, 3, 6, 9);
  c += c;
  check(c, 6, 12, 18);
  c += c.zyx;
  check(c, 24, 24, 24);
  c = a.xxx + b.zyx;
  check(c, 7, 6, 5);
  c.yzx += vec<int, 3>(2, 3, 1);
  check(c, 8, 8, 8);
}

TEST(vector_subtraction, vector_subtraction) {
  vec<int, 3> a(1, 2, 3);
  vec<int, 3> b(4, 5, 6);
  vec<int, 3> c = a - b;
  check(c, -3, -3, -3);
  c = a - a;
  check(c, 0, 0, 0);
  c -= a;
  check(c, -1, -2, -3);
  c -= c;
  check(c, 0, 0, 0);
  c -= b.xzy;
  check(c, -4, -6, -5);
  c = b.zzz - a;
  check(c, 5, 4, 3);
  c.zyx -= a.zyx;
  check(c, 4, 2, 0);
}

TEST(vector_negation, vector_negation) {
  vec<int, 3> a(1, 2, 3);
  vec<int, 3> b = -a;
  check(b, -1, -2, -3);
  check(a, 1, 2, 3);
  check(-a.zyx, -3, -2, -1);
  check(-(a.zyx), -3, -2, -1);
}

TEST(vector_multiplication, vector_multiplication) {
  vec<int, 3> a(1, 2, 3);
  vec<int, 3> b = a * 4;
  check(b, 4, 8, 12);
  check(a, 1, 2, 3);
  b *= 10;
  check(b, 40, 80, 120);
  check(2 * a, 2, 4, 6);
  check(a.yxz * 2, 4, 2, 6);
  check(2 * a.zzy, 6, 6, 4);
}

TEST(vector_division, vector_division) {
  vec<int, 3> a(3, 6, 9);
  vec<int, 3> b = a / 3;
  check(b, 1, 2, 3);
  b = a / 2.0;
  check(b, 1, 3, 4);
  check(a.zyx / 3, 3, 2, 1);
}

TEST(vector_dot_product, vector_dot_product) {
  vec<int, 3> a(1, 2, 3);
  vec<int, 3> b(4, 5, 6);
  int d = a.dot(b);
  EXPECT_EQ(32, d);
  check(a, 1, 2, 3);
  check(b, 4, 5, 6);
  EXPECT_EQ(18, a.zzz.dot(a));
}

TEST(vector_cross_product, vector_cross_product) {
  vec<int, 3> a(1, 0, 0);
  vec<int, 3> b(0, 1, 0);
  check(a.cross(b), 0, 0, 1);
  check(b.cross(a), 0, 0, -1);
  a = vec<int, 3>(1, 2, 3);
  check(a.cross(a), 0, 0, 0);
  check(a, 1, 2, 3);
  check(b.yxx.cross(b), 0, 0, 1);
  check(b.cross(b.yxz), 0, 0, -1);
}
