#pragma once

class Utility {
    public:
        template <typename T>
        static T min(const T& a, const T& b) noexcept { return ((a < b) ? a : b); }
        template <typename T>
        static T max(const T& a, const T& b) noexcept { return ((a > b) ? a : b); }

        template <typename T>
        static void swap(T& a, T& b) noexcept {
            T temp = a;

            a = b;
            b = temp;
        }
};