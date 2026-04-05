#pragma once

#include "types.hpp"

struct Vec4 {
    public:
        constexpr Vec4& operator*=(const f32 s) noexcept;

        constexpr Vec4 operator*(const f32 s) const noexcept;

    public:
        union {
            f32 data[4]{};

            struct { f32 x, y, z, w; };
            struct { f32 r, g, b, a; };
        };
};

constexpr Vec4& Vec4::operator*=(const f32 s) noexcept {
    x *= s;
    y *= s;
    z *= s;
    w *= s;

    return *this;
}

constexpr Vec4 Vec4::operator*(const f32 s) const noexcept {
    return { .data = { x * s, y * s, z * s, w * s } };
}