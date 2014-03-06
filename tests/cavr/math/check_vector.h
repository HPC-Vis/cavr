#pragma once

template<typename T, typename U>
void check_vector(const T& v,  const U& a) {
  EXPECT_EQ(a, v[0]);
}

template<typename T, typename U>
void check_vector(const T& v,  const U& a, const U& b) {
  EXPECT_EQ(b, v[1]);
  check_vector(v, a);
}

template<typename T, typename U>
void check_vector(const T& v,  const U& a, const U& b, const U& c) {
  EXPECT_EQ(c, v[2]);
  check_vector(v, a, b);
}

template<typename T, typename U>
void check_vector(const T& v,  const U& a, const U& b, const U& c, const U& d) {
  EXPECT_EQ(d, v[3]);
  check_vector(v, a, b, c);
}

