#pragma once

#include "world.hpp"

// fwd
struct MaterialDesc;
struct ObjectDesc;
struct LightDesc;
struct BuildContext;

namespace WorldBuilder {
    void buildMaterial(BuildContext& ctx, const MaterialDesc& desc);
    void buildObject(BuildContext& ctx, const ObjectDesc& desc);
    void buildLight(BuildContext& ctx, const LightDesc& desc);

    World buildWorld(const char* file);
}