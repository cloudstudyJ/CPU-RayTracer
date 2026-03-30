#pragma once

#include "types.hpp"

namespace Math {
    static constexpr f32 PI{3.141'592f};

    inline constexpr f32 toRad(const f32 degree) noexcept { return (PI / 180) * degree; }
    inline constexpr f32 toDegree(const f32 rad) noexcept { return (180 / PI) * rad; }

    f32 abs(const f32 val) noexcept;

    f32 pow(const f32 x, const f32 y) noexcept;
    f32 sqrt(const f32 val) noexcept;

    f32 sin(const f32 rad) noexcept;
    f32 cos(const f32 rad) noexcept;
    f32 tan(const f32 rad) noexcept;

    f32 arccos(const f32 val) noexcept;
    f32 arctan2(const f32 y, const f32 x) noexcept;
}