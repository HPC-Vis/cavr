#pragma once
#include <cavr/math/vector.h>
#include <vector>

namespace cavr {

namespace gfx {

class Shapes {
public:
  typedef std::vector<math::vec4f> VertexList;
  static VertexList solidSphere(int num_lats, int num_lons);
  static VertexList wireCube();
  static VertexList solidCone(int num_segments, float height, float radius);
  static VertexList solidCylinder(int num_segments,
                                  float height,
                                  float radius);
private:
};

} // namespace gfx

} // namespace cavr
