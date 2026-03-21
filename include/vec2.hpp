#pragma once

#include "types.hpp"

struct Vec2 {
    public:
        constexpr Vec2() noexcept = default;
        constexpr Vec2(const f32 a, const f32 b) noexcept: data{a, b} {}

    public:
        union {
            f32 data[2]{};

            struct { f32 x, y; };
            struct { f32 u, v; };
            struct { f32 s, t; };
        };
};