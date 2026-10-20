#pragma once

#include "math/vec3.hpp"

// fwd
class Sphere;
class Rectangle;

struct SurfaceSample {
    Vec3 point;
    Vec3 normal;
    f32  pdf   {};      // Area PDF
};

namespace sampling {
    SurfaceSample surface(const Sphere& sphere);
    SurfaceSample surface(const Rectangle& rectangle);
}