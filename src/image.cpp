#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace {
    const bool initSTBI = []() {
        stbi_set_flip_vertically_on_load(true);
        return true;
    }();
}

Image::Image(u32 width, u32 height, byte channels)
    : mWidth(width), mHeight(height), mChannels(channels), mData(new byte[width * height * channels]{}) {}
Image::~Image() noexcept { stbi_image_free(mData); }

bool Image::load(const char* file) {
    {
        i32 x, y, _;
        mData = stbi_load(file, &x, &y, &_, 3);

        if (!mData)
            return false;

        mWidth  = static_cast<u32>(x);
        mHeight = static_cast<u32>(y);
    }

    return true;
}

Vec3 Image::color(const u32 x, const u32 y) const noexcept {
    constexpr f32 INV_255{1.0f / 255};

    if (!mData || (x >= mWidth) || (y >= mHeight))
        return {1.0f, 0.0f, 1.0f};

    const u32 idx = (y * mWidth + x) * 3;

    return {
        static_cast<f32>(mData[idx    ]) * INV_255,
        static_cast<f32>(mData[idx + 1]) * INV_255,
        static_cast<f32>(mData[idx + 2]) * INV_255
    };
}