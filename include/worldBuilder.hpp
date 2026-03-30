#pragma once

#include "world.hpp"

#include <unordered_map>
#include <string>
#include <memory>

struct BuildContext {
    World& world;

    std::unordered_map<std::string, std::shared_ptr<Material>> materialTable;
    std::unordered_map<std::string, Object*>                   objectTable;
};

// fwd
struct MaterialDesc;
struct ObjectDesc;
struct LightDesc;
struct WorldDesc;

namespace WorldBuilder {
    void buildMaterial(BuildContext& ctx, const MaterialDesc& desc);
    void buildObject(BuildContext& ctx, const ObjectDesc& desc);
    void buildLight(BuildContext& ctx, const LightDesc& desc);

    World buildWorld(const WorldDesc& data);
}