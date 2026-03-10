#pragma once

namespace Utility {
    template <typename T>
    inline constexpr T min(const T& a, const T& b) noexcept { return ((a < b) ? a : b); }
    template <typename T>
    inline constexpr T max(const T& a, const T& b) noexcept { return ((a > b) ? a : b); }

    template <typename T>
    inline constexpr void swap(T& a, T& b) noexcept {
        T temp = a;

        a = b;
        b = temp;
    }

    template <typename T>
    inline constexpr bool isInRange(const T& val, const T& min, const T& max) noexcept { return (val >= min && val <= max); }
}