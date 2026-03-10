#pragma once

#include "hitResult.hpp"
#include "ray.hpp"

// fwd
struct AABB;

// fwd
class Sphere;

namespace Solver {
    HitResult hitTest(const Ray& ray, const Sphere& sphere, const float32 tMin, const float32 tMax);

    bool isIntersect(const Ray& ray, const AABB& aabb, const float32 tMin, const float32 tMax);
}