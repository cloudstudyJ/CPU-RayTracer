#pragma once

#include "types.hpp"

class Utility {
    public:
        static float32 min(const float32 a, const float32 b) noexcept { return ((a < b) ? a : b); }
        static float32 max(const float32 a, const float32 b) noexcept { return ((a > b) ? a : b); }
};