#pragma once

#include "vec3.hpp"

class Image {
    public:
        Image() noexcept;
        ~Image() noexcept;

        bool load(const char* file);

        Vec3 color(const uint32 x, const uint32 y) const noexcept;

        uint8 channels() const noexcept { return mChannels; }
        uint32 width() const noexcept { return mWidth; }
        uint32 height() const noexcept { return mHeight; }

    private:
        uint8 mChannels;

        uint32 mWidth;
        uint32 mHeight;

        uint8* mData;
};