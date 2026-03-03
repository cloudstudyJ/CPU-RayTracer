#pragma once

#include "types.hpp"

struct Vec3 {
    public:
        union {
            float32 data[3]{};

            struct { float32 x, y, z; };
            struct { float32 r, g, b; };
        };
};