#pragma once

#include "types.hpp"

namespace Math {
    constexpr f32 PI          = 3.141'592'653'589'793f;
    constexpr f32 TWO_PI      = 2 * PI;
    constexpr f32 FOUR_PI     = 4 * PI;
    constexpr f32 INV_PI      = 1 / PI;
    constexpr f32 INV_TWO_PI  = 1 / TWO_PI;
    constexpr f32 INV_FOUR_PI = 1 / FOUR_PI;

    constexpr f32 toRad(const f32 degree) noexcept;
    constexpr f32 toDegree(const f32 rad) noexcept;

    f32 abs(const f32 val) noexcept;
    f32 pow(const f32 x, const f32 y) noexcept;
    f32 sqrt(const f32 val) noexcept;
    f32 sin(const f32 rad) noexcept;
    f32 cos(const f32 rad) noexcept;
    f32 tan(const f32 rad) noexcept;
    f32 arccos(const f32 val) noexcept;
    f32 arctan2(const f32 y, const f32 x) noexcept;
}

inline constexpr f32 Math::toRad(const f32 degree) noexcept {
    return (PI / 180) * degree;
}
inline constexpr f32 Math::toDegree(const f32 rad) noexcept {
    return (180 * INV_PI) * rad;
}