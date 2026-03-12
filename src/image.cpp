#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Image::Image() noexcept: mWidth(0), mHeight(0), mChannels(3), mData(nullptr) {
    static const bool initOnce = []() {
        stbi_set_flip_vertically_on_load(true);
        return true;
    }();
}
Image::~Image() noexcept { stbi_image_free(mData); }

bool Image::load(const char* file) {
    {
        int x, y, _;
        mData = stbi_load(file, &x, &y, &_, 3);

        if (!mData)
            return false;

        mWidth = static_cast<uint32>(x);
        mHeight = static_cast<uint32>(y);
    }

    return true;
}

Vec3 Image::color(const uint32 x, const uint32 y) const noexcept {
    constexpr float32 INV_255 = 1.0f / 255;

    if (!mData || (x >= mWidth) || (y >= mHeight))
        return {1.0f, 0.0f, 1.0f};

    const uint32 idx = (y * mWidth + x) * 3;

    return {
        static_cast<float32>(mData[idx    ]) * INV_255,
        static_cast<float32>(mData[idx + 1]) * INV_255,
        static_cast<float32>(mData[idx + 2]) * INV_255
    };
}