#pragma once

template<typename T, typename U>
void check_vector(const T& v,  const U& a) {
  EXPECT_EQ(a, v[0]);
}

template<typename T, typename U>
void check_vector(const T& v,  const U& a, const U& b) {
  check_vector(v, a);
  EXPECT_EQ(b, v[1]);
}

template<typename T, typename U>
void check_vector(const T& v,  const U& a, const U& b, const U& c) {
  check_vector(v, a, b);
  EXPECT_EQ(c, v[2]);
}

template<typename T, typename U>
void check_vector(const T& v,  const U& a, const U& b, const U& c, const U& d) {
  check_vector(v, a, b, c);
  EXPECT_EQ(d, v[3]);
}

