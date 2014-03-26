#include <cavr/input/sixdof.h>
#include "gtest/gtest.h"
using cavr::input::SixDOF;
using cavr::math::vec3d;
using cavr::math::vec4d;
using cavr::math::autovec;

TEST(sixdof, sixdof) {
  SixDOF s;
  EXPECT_EQ(vec3d(0.0), s.getPosition());
  EXPECT_EQ(vec3d(1.0, 0.0, 0.0), s.getRight());
  EXPECT_EQ(vec3d(0.0, 1.0, 0.0), s.getUp());
  EXPECT_EQ(vec3d(0.0, 0.0, -1.0), s.getForward());
  EXPECT_EQ(vec3d(-1.0, 0.0, 0.0), s.getLeft());
  EXPECT_EQ(vec3d(0.0, -1.0, 0.0), s.getDown());
  EXPECT_EQ(vec3d(0.0, 0.0, 1.0), s.getBackward());
  cavr::math::mat4d m = s.getMatrix();
  EXPECT_EQ(vec4d(1.0, 0.0, 0.0, 0.0), m[0]);
  EXPECT_EQ(vec4d(0.0, 1.0, 0.0, 0.0), m[1]);
  EXPECT_EQ(vec4d(0.0, 0.0, 1.0, 0.0), m[2]);
  EXPECT_EQ(vec4d(0.0, 0.0, 0.0, 1.0), m[3]);

  cavr::math::mat4d new_state = 
    cavr::math::mat4d::look_at(autovec(1.0, 2.0, 3.0),
                               autovec(4, 5, 6),
                               autovec(0, 1, 0)).inverted();
  s.setState(new_state);
  m = s.getMatrix();
  EXPECT_EQ(new_state[0], m[0]);
  EXPECT_EQ(new_state[1], m[1]);
  EXPECT_EQ(new_state[2], m[2]);
  EXPECT_EQ(new_state[3], m[3]);
  vec3d p = s.getPosition();
  EXPECT_DOUBLE_EQ(1.0, p.x);
  EXPECT_DOUBLE_EQ(2.0, p.y);
  EXPECT_DOUBLE_EQ(3.0, p.z);
}
