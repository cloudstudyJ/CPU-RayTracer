#pragma once

#include "texture.hpp"

#include <memory>

// fwd
class Image;

class ImageTexture: public Texture {
    ImageTexture(const ImageTexture&)                = delete;
    ImageTexture(ImageTexture&&) noexcept            = delete;
    ImageTexture& operator=(const ImageTexture&)     = delete;
    ImageTexture& operator=(ImageTexture&&) noexcept = delete;

    public:
        ImageTexture() noexcept = default;
        explicit ImageTexture(std::shared_ptr<Image> image) noexcept;
        ~ImageTexture() noexcept override = default;

        Vec3 sample(const Vec2& uv) const noexcept override;

    private:
        std::shared_ptr<Image> mImage;
};