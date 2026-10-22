#include "material/metal.hpp"
#include "bxdfs/ggxReflection.hpp"
#include "texture.hpp"

Metal::Metal(std::shared_ptr<Texture> texture, const f32 roughness) noexcept
    : Material()
    , mRoughness(roughness)
    , mTexture(std::move(texture)) { }

BxDFSample Metal::interact(const HitResult& hitResult, const Vec3& wo) const {
    return GGXReflection(mTexture->color(hitResult.u, hitResult.v), mRoughness).sample(wo);
}