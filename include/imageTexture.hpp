#pragma once

#include "texture.hpp"

#include <memory>

// fwd
class Image;

class ImageTexture: public Texture {
    public:
        ImageTexture() noexcept: mImage(nullptr) {}
        ImageTexture(std::shared_ptr<Image> image) noexcept: mImage(image) {}

        Vec3 color(const float32 u, const float32 v) const noexcept override;

    private:
        std::shared_ptr<Image> mImage;
};