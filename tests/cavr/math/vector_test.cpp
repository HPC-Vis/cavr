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
}

TEST(vector_negation, vector_negation) {
  vec<int, 3> a(1, 2, 3);
  vec<int, 3> b = -a;
  check(b, -1, -2, -3);
  check(a, 1, 2, 3);
}

class swizzle_tests
  : public ::testing::Test {
public:
  swizzle_tests()
    : v12(1,2),
      v34(3,4),
      v567(5,6,7),
      v890(8,9,0) {
  }

  vec<int, 2> v12, v34;
  vec<int, 3> v567, v890;
};

TEST_F(swizzle_tests, vector_construction) {
  vec<int, 4> a(v12.xx, v12.yy);
  check(a, 1, 1, 2, 2);

  vec<int, 4> b(v12.xy, v34.xy);
  check(b, 1, 2, 3, 4);

  vec<int, 3> c(v567.zyx);
  check(c, 7, 6, 5);

  vec<int, 3> d(v12.yyy);
  check(d, 2, 2, 2);

  vec<int, 4> e(0, v12.xxx);
  check(e, 0, 1, 1, 1);
}

