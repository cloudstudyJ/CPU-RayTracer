#pragma once

#include "types.hpp"

class Math {
    public:
        static float32 toRad(const float32 degree) noexcept { return (PI / 180) * degree; }
        static float32 toDegree(const float32 rad) noexcept { return (180 / PI) * rad; }

        static float32 pow(const float32 x, const float32 y) noexcept;
        static float32 sqrt(const float32 val) noexcept;

        static float32 tan(const float32 degree) noexcept;

    public:
        static constexpr float32 PI = 3.141'592f;
};