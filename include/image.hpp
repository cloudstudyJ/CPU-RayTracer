#pragma once

#include "vec3.hpp"

class Image {
    public:
        Image() noexcept;
        ~Image() noexcept;

        bool load(const char* file);

        Vec3 color(const u32 x, const u32 y) const noexcept;

        byte channels() const noexcept { return mChannels; }
        u32 width() const noexcept { return mWidth; }
        u32 height() const noexcept { return mHeight; }

    private:
        byte mChannels;

        u32 mWidth;
        u32 mHeight;

        byte* mData;
};