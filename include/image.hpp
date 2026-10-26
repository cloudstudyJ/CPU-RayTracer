#pragma once

#include "math/vec3.hpp"

class Image {
    Image(const Image&)            = delete;
    Image& operator=(const Image&) = delete;

    public:
        Image() noexcept = default;
        Image(Image&& other) noexcept;
        ~Image() noexcept;

        Image& operator=(Image&& other) noexcept;

        bool load(const char* file);

        Vec3 color(const u32 x, const u32 y) const noexcept;
        byte channels() const noexcept;
        u32  width() const noexcept;
        u32  height() const noexcept ;

    private:
        byte mChannels{};
        u32  mWidth   {};
        u32  mHeight  {};

        byte* mData{};
};