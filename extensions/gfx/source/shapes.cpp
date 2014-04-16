#include <cavr/gfx/shapes.h>
#include <algorithm>

namespace cavr {

namespace gfx {

Shapes::VertexList Shapes::solidSphere(int num_lats, int num_lons) {
  using math::vec4f;
  VertexList result;
  float rads_per_lat = M_PI / float(num_lats);
  float rads_per_lon = 2.0 * M_PI / float(num_lons);
  for (int lat = 0; lat < num_lats; ++lat) {
    float top_rad = rads_per_lat * lat - M_PI / 2.0f;
    float bottom_rad = top_rad + rads_per_lat;
    float top_y = sin(top_rad);
    float top_xz = cos(top_rad);
    float bottom_y = sin(bottom_rad);
    float bottom_xz = cos(bottom_rad);
    for (int lon = 0; lon < num_lons; ++lon) {
      float left_rad = rads_per_lon * lon;
      float right_rad = left_rad + rads_per_lon;
      float sin_l = sin(left_rad);
      float cos_l = cos(left_rad);
      float sin_r = sin(right_rad);
      float cos_r = cos(right_rad);
      vec4f ll(bottom_xz * cos_l, bottom_y, bottom_xz * sin_l, 1.0);
      vec4f lr(bottom_xz * cos_r, bottom_y, bottom_xz * sin_r, 1.0);
      vec4f ul(top_xz * cos_l, top_y, top_xz * sin_l, 1.0);
      vec4f ur(top_xz * cos_r, top_y, top_xz * sin_r, 1.0);
      result.push_back(ll);
      result.push_back(lr);
      result.push_back(ul);
      result.push_back(ul);
      result.push_back(lr);
      result.push_back(ur);
    }
  }
  return result;
}

Shapes::VertexList Shapes::wireCube() {
  using math::vec4f;
  vec4f left_bottom_front(-1, -1, -1, 1);
  vec4f right_bottom_front(1, -1, -1, 1);
  vec4f left_top_front(-1, 1, -1, 1);
  vec4f right_top_front(1, 1, -1, 1);
  vec4f left_bottom_back(-1, -1, 1, 1);
  vec4f right_bottom_back(1, -1, 1, 1);
  vec4f left_top_back(-1, 1, 1, 1);
  vec4f right_top_back(1, 1, 1, 1);
  VertexList cube_lines({
    // left to right
    left_bottom_front, right_bottom_front,
    left_top_front, right_top_front,
    left_bottom_back, right_bottom_back,
    left_top_back, right_top_back,
    // bottom to top
    left_bottom_front, left_top_front,
    right_bottom_front, right_top_front,
    left_bottom_back, left_top_back,
    right_bottom_back, right_top_back,
    // front to back
    left_bottom_back, left_bottom_front,
    right_bottom_back, right_bottom_front,
    left_top_back, left_top_front,
    right_top_back, right_top_front
  });
  return cube_lines;
}

Shapes::VertexList Shapes::solidCone(int num_segments,
                                     float height,
                                     float radius) {
  using math::vec4f;
  VertexList result;
  vec4f center(0, 0, 0, 1);
  vec4f tip(0, 0, height, 1);
  float rads_per_segment = 2.0 * M_PI / double(num_segments);
  for (int i = 0; i < num_segments; ++i) {
    float r1 = rads_per_segment * i;
    float r2 = r1 + rads_per_segment;
    vec4f r(radius * cos(r1), radius * sin(r1), 0, 1);
    vec4f l(radius * cos(r2), radius * sin(r2), 0, 1);
    result.push_back(center);
    result.push_back(l);
    result.push_back(r);
    result.push_back(r);
    result.push_back(l);
    result.push_back(tip);
  }
  if (height < 0.0) {
    std::reverse(result.begin(), result.end());
  }
  return result;
}

Shapes::VertexList Shapes::solidCylinder(int num_segments,
                                         float height,
                                         float radius) {
  using math::vec4f;
  VertexList result;
  vec4f bottom_center(0, 0, 0, 1);
  vec4f top_center(0, 0, height, 1);
  float rads_per_segment = 2.0 * M_PI / double(num_segments);
  for (int i = 0; i < num_segments; ++i) {
    float r1 = rads_per_segment * i;
    float r2 = r1 + rads_per_segment;
    vec4f bottom_r(radius * cos(r1), radius * sin(r1), 0, 1);
    vec4f bottom_l(radius * cos(r2), radius * sin(r2), 0, 1);
    vec4f top_r(radius * cos(r1), radius * sin(r1), height, 1);
    vec4f top_l(radius * cos(r2), radius * sin(r2), height, 1);
    result.push_back(bottom_center);
    result.push_back(bottom_l);
    result.push_back(bottom_r);
    result.push_back(top_center);
    result.push_back(top_r);
    result.push_back(top_l);
    result.push_back(bottom_r);
    result.push_back(bottom_l);
    result.push_back(top_r);
    result.push_back(top_r);
    result.push_back(bottom_l);
    result.push_back(top_l);
  }
  return result;
}

} // namespace gfx

} // namespace cavr

