#include "worldBuilder.hpp"
#include "worldLoader.hpp"
#include "objects.hpp"
#include "solidTexture.hpp"
#include "imageTexture.hpp"
#include "image.hpp"
#include "material/diffuse.hpp"
#include "material/metal.hpp"

#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

struct BuildContext {
    World& world;

    std::unordered_map<std::string, std::shared_ptr<Material>> materialTable;
    std::unordered_map<std::string, Object*>                   objectTable;
};

Light::SampleFn getLightSampler(const Object& obj) noexcept {
    if (dynamic_cast<const Sphere*>(&obj))
        return lightSurfaceSampler<Sphere>;

    if (dynamic_cast<const Rectangle*>(&obj))
        return lightSurfaceSampler<Rectangle>;

    return nullptr;
}
Light::PDFFn getLightSurfacePDF(const Object& obj) noexcept {
    if (dynamic_cast<const Sphere*>(&obj))
        return lightSurfacePDF<Sphere>;

    if (dynamic_cast<const Rectangle*>(&obj))
        return lightSurfacePDF<Rectangle>;

    return nullptr;
}

std::shared_ptr<Texture> createTexture(const TextureDesc& desc) {
    std::shared_ptr<Texture> texture;

    if (desc.src == TextureSource::Constant)
        return std::make_shared<SolidTexture>(std::get<ConstantTextureDesc>(desc.data).color);

    else if (desc.src == TextureSource::Image) {
        const auto& file = std::get<ImageTextureDesc>(desc.data).file;
        auto        img  = std::make_shared<Image>();

        if (!img->load(file.c_str())) {
            std::cout << "Failed to load Texture Image: " << file << '\n';
            return nullptr;
        }

        return std::make_shared<ImageTexture>(std::move(img));
    }

    return nullptr;
}

namespace WorldBuilder {
    void buildMaterial(BuildContext& ctx, const MaterialDesc& desc) {
        std::shared_ptr<Texture>  baseColor, normal, orm;
        std::shared_ptr<Material> material;

        for (const auto& texDesc: desc.textures) {
            std::shared_ptr<Texture> texture = createTexture(texDesc);

            switch (texDesc.type) {
                case TextureType::BaseColor:
                    baseColor = std::move(texture);
                    break;

                case TextureType::Normal:
                    normal = std::move(texture);
                    break;

                case TextureType::ORM:
                    orm = std::move(texture);
                    break;

                default:
                    break;
            }
        }

        if (!orm)
            orm = std::make_shared<SolidTexture>(Vec3{ 1.0f, 1.0f, 1.0f });

        if (desc.bxdfType == BxDFType::Lambertian)
            material = std::make_shared<Diffuse>(baseColor, normal);

        else if (desc.bxdfType == BxDFType::Metal)
            material = std::make_shared<Metal>(baseColor, normal, orm, std::get<MetalDesc>(desc.bxdfDesc).metallic, std::get<MetalDesc>(desc.bxdfDesc).roughness);

        /* else if (desc.bxdfType == BxDFType::Dielectric)
            material = std::make_shared<Dielectric>(); */

        ctx.materialTable.try_emplace(desc.name, material);
    }
    void buildObject(BuildContext& ctx, const ObjectDesc& desc) {
        const auto& geoData = desc.geometryData;
        const auto& geoType = desc.geometryType;

        std::shared_ptr<Material> material;
        if (!desc.materialName.empty() && ctx.materialTable.contains(desc.materialName))
            material = ctx.materialTable.find(desc.materialName)->second;

        Object* newObject{};
        if      (geoType == GeometryType::Sphere   ) newObject = ctx.world.addObject<Sphere>(desc.transform, std::get<SphereDesc>(geoData).radius, material);
        else if (geoType == GeometryType::Rectangle) newObject = ctx.world.addObject<Rectangle>(desc.transform, material);
        else if (geoType == GeometryType::Cube     ) newObject = ctx.world.addObject<Cube>(desc.transform, material);

        ctx.objectTable.try_emplace(desc.name, newObject);
    }
    void buildLight(BuildContext& ctx, const LightDesc& desc) {
        auto it = ctx.objectTable.find(desc.objectName);
        if (it == ctx.objectTable.end())
            return;

        ctx.world.addLight(
            it->second,
            {
                .color     = desc.color,
                .intensity = desc.intensity,
                .sample    = getLightSampler(*it->second),
                .pdf       = getLightSurfacePDF(*it->second)
            }
        );
    }

    World buildWorld(const char* file) {
        WorldDesc desc = WorldLoader::parseWorld(file);

        World world;
        BuildContext ctx = { world };

        world.addCamera(desc.camera);
        for (const auto& material: desc.materials) buildMaterial(ctx, material);
        for (const auto& object  : desc.objects  ) buildObject(ctx, object);
        for (const auto& light   : desc.lights   ) buildLight(ctx, light);
        world.buildAccelerationStructure();

        return world;
    }
}
