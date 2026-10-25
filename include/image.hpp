#pragma once

#include "math/vec3.hpp"

class Image {
    Image(const Image&)                = delete;
    Image(Image&&) noexcept            = delete;
    Image& operator=(const Image&)     = delete;
    Image& operator=(Image&&) noexcept = delete;

    public:
        Image() noexcept = default;
        ~Image() noexcept;

        bool load(const char* file);

        Vec3 color(const u32 x, const u32 y) const noexcept;

        byte channels() const noexcept { return mChannels; }
        u32 width() const noexcept { return mWidth; }
        u32 height() const noexcept { return mHeight; }

    private:
        byte mChannels{ 4 };
        u32  mWidth   {};
        u32  mHeight  {};

        byte* mData{};
};