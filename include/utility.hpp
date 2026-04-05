#pragma once

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