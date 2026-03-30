#include "worldBuilder.hpp"
#include "worldLoader.hpp"
#include "objects.hpp"
#include "bxdfs.hpp"
#include "solidTexture.hpp"

namespace WorldBuilder {
    void buildMaterial(BuildContext& ctx, const MaterialDesc& desc) {
        auto material = std::make_shared<Material>();
        if      (desc.bxdfType == BxDFType::Lambertian) material->bxdf = std::make_shared<Lambertian>();
        else if (desc.bxdfType == BxDFType::Metal     ) material->bxdf = std::make_shared<Metal>(std::get<MetalDesc>(desc.bxdfData).roughness);
        else if (desc.bxdfType == BxDFType::Dielectric) material->bxdf = std::make_shared<Dielectric>(std::get<DielectricDesc>(desc.bxdfData).refractiveIndex);

        if (desc.textureType == TextureType::Solid) material->texture = std::make_shared<SolidTexture>(std::get<SolidTextureDesc>(desc.textureData).color);
        // Todo: ImageTexture

        ctx.materialTable.try_emplace(desc.name, material);
    }
    void buildObject(BuildContext& ctx, const ObjectDesc& desc) {
        const auto& geoData = desc.geometryData;

        std::shared_ptr<Material> material;
        if (!desc.materialName.empty() && ctx.materialTable.contains(desc.materialName))
            material = ctx.materialTable.find(desc.materialName)->second;

        Object* newObject = {};
        if      (desc.geometryType == GeometryType::Sphere) newObject = ctx.world.addObject<Sphere>(desc.transform.position, std::get<SphereDesc>(geoData).radius, material);
        else if (desc.geometryType == GeometryType::Quad  ) newObject = ctx.world.addObject<Quad>(desc.transform.position, std::get<QuadDesc>(geoData).u, std::get<QuadDesc>(geoData).v, material);
        else if (desc.geometryType == GeometryType::Box   ) newObject = ctx.world.addObject<Box>(std::get<BoxDesc>(geoData).p1, std::get<BoxDesc>(geoData).p2, material);

        ctx.objectTable.try_emplace(desc.name, newObject);
    }
    void buildLight(BuildContext& ctx, const LightDesc& desc) {
        auto it = ctx.objectTable.find(desc.objectName);
        if (it == ctx.objectTable.end())
            return;

        ctx.world.addLight(it->second, {desc.color, desc.intensity});
    }

    World buildWorld(const WorldDesc& data) {
        World world;

        BuildContext ctx = {world};
        for (const auto& material: data.materials) buildMaterial(ctx, material);
        for (const auto& object  : data.objects  ) buildObject(ctx, object);
        for (const auto& light   : data.lights   ) buildLight(ctx, light);
        world.buildAccelerationStructure();

        return world;
    }
}