#pragma once

#include "types.hpp"

struct Vec2 {
    public:
        union {
            f32 data[2]{};

            struct { f32 x, y; };
            struct { f32 u, v; };
            struct { f32 s, t; };
        };
};