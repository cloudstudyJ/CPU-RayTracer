#include "imageTexture.hpp"
#include "image.hpp"

ImageTexture::ImageTexture(std::shared_ptr<Image> image) noexcept
    : Texture()
    , mImage(std::move(image)) { }

Vec3 ImageTexture::sample(const Vec2& uv) const noexcept {
    return mImage->color(
        uv.u * (mImage->width() - 1),
        uv.v * (mImage->height() - 1)
    );
}