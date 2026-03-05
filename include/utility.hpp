#pragma once

#include "types.hpp"

class Utility {
    public:
        // [0.0, 1.0)
        static float32 random() noexcept;
        // [from, to)
        static float32 random(const float32 from, const float32 to) noexcept { return (from + (to - from) * random()); }

        template <typename T>
        static T min(const T& a, const T& b) noexcept { return ((a < b) ? a : b); }
        template <typename T>
        static T max(const T& a, const T& b) noexcept { return ((a > b) ? a : b); }

        static void swap(float32& a, float32& b) noexcept {
            float32 temp = a;

            a = b;
            b = temp;
        }
};