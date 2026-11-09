#pragma once

#include "math/vec3.hpp"

namespace Utility {
    template <typename T>
    constexpr T min(const T& a, const T& b) noexcept;
    template <typename T>
    constexpr T max(const T& a, const T& b) noexcept;
    template <typename T>
    constexpr T clamp(const T& val, const T& low, const T& high) noexcept;

    template <typename T>
    constexpr void swap(T& a, T& b) noexcept;

    template <typename T>
    constexpr bool isInClosedRange(const T& val, const T& min, const T& max) noexcept;

    // Linear RGB to Luminance (Rec. 709 | sRGB)
    constexpr f32 luminance(const Vec3& color) noexcept;
}

template <typename T>
constexpr T Utility::min(const T& a, const T& b) noexcept {
    return (a < b) ? a : b;
}
template <typename T>
constexpr T Utility::max(const T& a, const T& b) noexcept {
    return (a > b) ? a : b;
}
template <typename T>
constexpr T Utility::clamp(const T& val, const T& low, const T& high) noexcept {
    return min(max(val, low), high);
}

template <typename T>
constexpr void Utility::swap(T& a, T& b) noexcept {
    T temp = a;

    a = b;
    b = temp;
}

template <typename T>
constexpr bool Utility::isInClosedRange(const T& val, const T& min, const T& max) noexcept {
    return val >= min && val <= max;
}

constexpr f32 Utility::luminance(const Vec3& color) noexcept {
    return (
        0.2126f * color.r +
        0.7152f * color.g +
        0.0722f * color.b
    );
}