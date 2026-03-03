#pragma once

#include "hitResult.hpp"
#include "ray.hpp"
#include "sphere.hpp"

class Solver {
    public:
        static HitResult hitTest(const Ray& ray, const Sphere& sphere) noexcept;
};