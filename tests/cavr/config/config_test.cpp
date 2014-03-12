#include<cavr/config/config.h>
#include<cavr/math/matrix.h>
#include<cavr/math/vector.h>
#include "gtest/gtest.h"

using cavr::config::transform;
using cavr::math::mat4d;
using cavr::math::autovec;

TEST(transform, construction) {
  transform a;
  transform b(a);
  EXPECT_EQ(a.matrix()->columns[0], b.matrix()->columns[0]);
  EXPECT_EQ(a.matrix()->columns[1], b.matrix()->columns[1]);
  EXPECT_EQ(a.matrix()->columns[2], b.matrix()->columns[2]);
  EXPECT_EQ(a.matrix()->columns[3], b.matrix()->columns[3]);
  mat4d c = mat4d::look_at(autovec(1, 1, 1),
                           autovec(-1, -1, 3),
                           autovec(0, -1, 0));
  transform d(c);
  EXPECT_EQ(c[0], d.matrix()->columns[0]);
  EXPECT_EQ(c[1], d.matrix()->columns[1]);
  EXPECT_EQ(c[2], d.matrix()->columns[2]);
  EXPECT_EQ(c[3], d.matrix()->columns[3]);
}

TEST(transform, assignment) {
  transform a(mat4d::look_at(autovec(1, 1, 1),
                             autovec(-1, -1, 3),
                             autovec(0, -1, 0)));
  transform b(mat4d::ortho(-2, 0, 1, 2, 4, 7));
  a = b;
  EXPECT_EQ(a.matrix()->columns[0], b.matrix()->columns[0]);
  EXPECT_EQ(a.matrix()->columns[1], b.matrix()->columns[1]);
  EXPECT_EQ(a.matrix()->columns[2], b.matrix()->columns[2]);
  EXPECT_EQ(a.matrix()->columns[3], b.matrix()->columns[3]);
}

TEST(transform, matrix_multiplication) {
  mat4d a = mat4d::perspective(1.1, 0.2, 1, 5);
  mat4d b = mat4d::rotate(2.2, 1, 2, 3);
  transform x(a);
  transform y(b);
  mat4d c = a * b;
  transform z = x * y;
  EXPECT_EQ(c[0], z.matrix()->columns[0]);
  EXPECT_EQ(c[1], z.matrix()->columns[1]);
  EXPECT_EQ(c[2], z.matrix()->columns[2]);
  EXPECT_EQ(c[3], z.matrix()->columns[3]);
  c *= a;
  z *= x;
  EXPECT_EQ(c[0], z.matrix()->columns[0]);
  EXPECT_EQ(c[1], z.matrix()->columns[1]);
  EXPECT_EQ(c[2], z.matrix()->columns[2]);
  EXPECT_EQ(c[3], z.matrix()->columns[3]);
}

TEST(transform, scalar_multiplication) {
  mat4d a = mat4d::perspective(1.1, 0.2, 1, 5);
  transform x(a);
  mat4d b = a * 47.2;
  transform y = x * 47.2;
  EXPECT_EQ(b[0], y.matrix()->columns[0]);
  EXPECT_EQ(b[1], y.matrix()->columns[1]);
  EXPECT_EQ(b[2], y.matrix()->columns[2]);
  EXPECT_EQ(b[3], y.matrix()->columns[3]);
  b *= 22.1;
  y *= 22.1;
  EXPECT_EQ(b[0], y.matrix()->columns[0]);
  EXPECT_EQ(b[1], y.matrix()->columns[1]);
  EXPECT_EQ(b[2], y.matrix()->columns[2]);
  EXPECT_EQ(b[3], y.matrix()->columns[3]);
  mat4d c = 99.2 * b;
  transform z = 99.2 * y;
  EXPECT_EQ(c[0], z.matrix()->columns[0]);
  EXPECT_EQ(c[1], z.matrix()->columns[1]);
  EXPECT_EQ(c[2], z.matrix()->columns[2]);
  EXPECT_EQ(c[3], z.matrix()->columns[3]);
}

TEST(transform, division) {
  mat4d a = mat4d::perspective(1.1, 0.2, 1, 5);
  transform x(a);
  mat4d b = a / 47.2;
  transform y = x / 47.2;
  EXPECT_EQ(b[0], y.matrix()->columns[0]);
  EXPECT_EQ(b[1], y.matrix()->columns[1]);
  EXPECT_EQ(b[2], y.matrix()->columns[2]);
  EXPECT_EQ(b[3], y.matrix()->columns[3]);
  b /= 22.1;
  y /= 22.1;
  EXPECT_EQ(b[0], y.matrix()->columns[0]);
  EXPECT_EQ(b[1], y.matrix()->columns[1]);
  EXPECT_EQ(b[2], y.matrix()->columns[2]);
  EXPECT_EQ(b[3], y.matrix()->columns[3]);
}

TEST(transform, identity) {
  mat4d a(1.0);
  transform t = cavr::config::identity();
  EXPECT_EQ(a[0], t.matrix()->columns[0]);
  EXPECT_EQ(a[1], t.matrix()->columns[1]);
  EXPECT_EQ(a[2], t.matrix()->columns[2]);
  EXPECT_EQ(a[3], t.matrix()->columns[3]);
}

TEST(transform, translate) {
  mat4d a = mat4d::translate(2, 5, 22.1);
  transform t = cavr::config::translate(2, 5, 22.1);
  EXPECT_EQ(a[0], t.matrix()->columns[0]);
  EXPECT_EQ(a[1], t.matrix()->columns[1]);
  EXPECT_EQ(a[2], t.matrix()->columns[2]);
  EXPECT_EQ(a[3], t.matrix()->columns[3]);
}

TEST(transform, rotate) {
  mat4d a = mat4d::rotate(1.4, 3, 2, 1);
  transform t = cavr::config::rotate(1.4, 3, 2, 1);
  EXPECT_EQ(a[0], t.matrix()->columns[0]);
  EXPECT_EQ(a[1], t.matrix()->columns[1]);
  EXPECT_EQ(a[2], t.matrix()->columns[2]);
  EXPECT_EQ(a[3], t.matrix()->columns[3]);
}

TEST(transform, scale) {
  mat4d a(1.0);
  a[0][0] = 1.2;
  a[1][1] = 3.4;
  a[2][2] = 5.6;
  transform t = cavr::config::scale(1.2, 3.4, 5.6);
  EXPECT_EQ(a[0], t.matrix()->columns[0]);
  EXPECT_EQ(a[1], t.matrix()->columns[1]);
  EXPECT_EQ(a[2], t.matrix()->columns[2]);
  EXPECT_EQ(a[3], t.matrix()->columns[3]);
}
