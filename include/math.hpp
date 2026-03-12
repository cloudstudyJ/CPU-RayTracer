#pragma once

#include "types.hpp"

namespace Math {
    static constexpr float32 PI = 3.141'592f;

    inline constexpr float32 toRad(const float32 degree) noexcept { return (PI / 180) * degree; }
    inline constexpr float32 toDegree(const float32 rad) noexcept { return (180 / PI) * rad; }

    float32 pow(const float32 x, const float32 y) noexcept;
    float32 sqrt(const float32 val) noexcept;

    float32 tan(const float32 degree) noexcept;
    float32 arccos(const float32 val) noexcept;
    float32 arctan2(const float32 y, const float32 x) noexcept;
}