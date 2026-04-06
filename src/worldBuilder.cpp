#include "worldBuilder.hpp"
#include "worldLoader.hpp"
#include "objects.hpp"
#include "bxdfs.hpp"
#include "solidTexture.hpp"

#include <unordered_map>
#include <string>
#include <memory>

struct BuildContext {
    World& world;

    std::unordered_map<std::string, std::shared_ptr<Material>> materialTable;
    std::unordered_map<std::string, Object*>                   objectTable;
};

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
        const auto& geoType = desc.geometryType;

        std::shared_ptr<Material> material;
        if (!desc.materialName.empty() && ctx.materialTable.contains(desc.materialName))
            material = ctx.materialTable.find(desc.materialName)->second;

        Object* newObject = {};
        if      (geoType == GeometryType::Sphere   ) newObject = ctx.world.addObject<Sphere>(desc.transform, std::get<SphereDesc>(geoData).radius, material);
        else if (geoType == GeometryType::Rectangle) newObject = ctx.world.addObject<Rectangle>(desc.transform, material);
        else if (geoType == GeometryType::Cube     ) newObject = ctx.world.addObject<Cube>(desc.transform, material);

        ctx.objectTable.try_emplace(desc.name, newObject);
    }
    void buildLight(BuildContext& ctx, const LightDesc& desc) {
        auto it = ctx.objectTable.find(desc.objectName);
        if (it == ctx.objectTable.end())
            return;

        ctx.world.addLight(it->second, {desc.color, desc.intensity});
    }

    World buildWorld(const char* file) {
        WorldDesc desc = WorldLoader::parseWorld(file);

        World world;
        BuildContext ctx = {world};

        world.addCamera(desc.camera);
        for (const auto& material: desc.materials) buildMaterial(ctx, material);
        for (const auto& object  : desc.objects  ) buildObject(ctx, object);
        for (const auto& light   : desc.lights   ) buildLight(ctx, light);
        world.buildAccelerationStructure();

        return world;
    }
}