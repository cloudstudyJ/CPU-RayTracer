#pragma once

#include "types.hpp"

#include <random>

class Utility {
    public:
        // [0.0, 1.0)
        static float32 random() noexcept;
        // [from, to)
        static float32 random(const float32 from, const float32 to) noexcept { return (from + (to - from) * random()); }

        static float32 min(const float32 a, const float32 b) noexcept { return ((a < b) ? a : b); }
        static float32 max(const float32 a, const float32 b) noexcept { return ((a > b) ? a : b); }

        static void swap(float32& a, float32& b) noexcept {
            float32 temp = a;

            a = b;
            b = temp;
        }

    private:
        static std::random_device rd;
        static std::mt19937       gen;
};