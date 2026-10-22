#include "material/diffuse.hpp"
#include "bxdfs/lambertian.hpp"
#include "texture.hpp"

Diffuse::Diffuse(std::shared_ptr<Texture> texture) noexcept
    : Material()
    , mTexture(std::move(texture)) { }

BxDFSample Diffuse::interact(const HitResult& hitResult, const Vec3& wo) const {
    return Lambertian(mTexture->color(hitResult.u, hitResult.v)).sample(wo);
}