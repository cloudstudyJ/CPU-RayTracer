#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <utility>

namespace {
    const bool initSTBI = []() {
        stbi_set_flip_vertically_on_load(true);
        return true;
    }();
}

Image::Image(Image&& other) noexcept
    : mChannels(std::exchange(other.mChannels, 0))
    , mWidth(std::exchange(other.mWidth, 0))
    , mHeight(std::exchange(other.mHeight, 0))
    , mData(std::exchange(other.mData, nullptr)) { }
Image::~Image() noexcept { stbi_image_free(mData); }

Image& Image::operator=(Image&& other) noexcept {
    if (this == &other)
        return *this;

    stbi_image_free(mData);

    mChannels = std::exchange(other.mChannels, 0);
    mWidth    = std::exchange(other.mWidth, 0);
    mHeight   = std::exchange(other.mHeight, 0);
    mData     = std::exchange(other.mData, nullptr);

    return *this;
}

bool Image::load(const char* file) {
    {
        i32 x, y, _;
        byte* imgData = stbi_load(file, &x, &y, &_, 4);

        if (!imgData)
            return false;

        stbi_image_free(mData);

        mChannels = 4;
        mWidth    = static_cast<u32>(x);
        mHeight   = static_cast<u32>(y);
        mData     = imgData;
    }

    return true;
}

Vec3 Image::color(const u32 x, const u32 y) const noexcept {
    constexpr f32 INV_255 = 1.0f / 255;

    if (!mData || (x >= mWidth) || (y >= mHeight))
        return { 1.0f, 0.0f, 1.0f };    // Magenta

    const u32 idx = (y * mWidth + x) * 4;

    return {
        static_cast<f32>(mData[idx    ]) * INV_255,
        static_cast<f32>(mData[idx + 1]) * INV_255,
        static_cast<f32>(mData[idx + 2]) * INV_255
    };
}
byte Image::channels() const noexcept {
    return mChannels;
}
u32 Image::width() const noexcept {
    return mWidth;
}
u32 Image::height() const noexcept {
    return mHeight;
}