#include <cavr/math/vector.h>
#include "gtest/gtest.h"
using cavr::math::vec;

TEST(vector_construction, default_constructor) {
  vec<int, 3> a;
  vec<float, 2> b;
  vec<char, 4> c;
}

