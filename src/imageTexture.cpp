#include "imageTexture.hpp"
#include "image.hpp"

Vec3 ImageTexture::color(const float32 u, const float32 v) const noexcept {
    return mImage->color(
        u * (mImage->width() - 1),
        v * (mImage->height() - 1)
    );
}