#pragma once

#include "hitResult.hpp"

// fwd
struct Ray;
struct AABB;

// fwd
class Sphere;
class Rectangle;

namespace Solver {
    HitResult hitTest(const Ray& ray, const Sphere& sphere, const f32 tMin, const f32 tMax);
    HitResult hitTest(const Ray& ray, const Rectangle& rect, const f32 tMin, const f32 tMax);

    bool isIntersect(const Ray& ray, const AABB& aabb, const f32 tMin, const f32 tMax);
}