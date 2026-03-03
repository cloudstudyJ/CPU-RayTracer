#pragma once

#include "hitResult.hpp"
#include "ray.hpp"
#include "sphere.hpp"

class Solver {
    public:
        static HitResult hitTest(const Ray& ray, const Sphere& sphere) noexcept;

    public:
        static constexpr float32 T_MIN = 0.001f;
};