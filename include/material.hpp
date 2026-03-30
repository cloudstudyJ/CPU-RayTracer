#pragma once

#include <memory>

// fwd
class BxDF;
class Texture;

struct Material {
    std::shared_ptr<BxDF>    bxdf;
    std::shared_ptr<Texture> texture;
};