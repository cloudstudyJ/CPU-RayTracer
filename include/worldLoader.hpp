#pragma once

#include "worldTypes.hpp"
#include "jsonParser.hpp"

namespace WorldLoader {
    MaterialDesc parseMaterial(const jsonValue& material);
    ObjectDesc   parseObject(const jsonValue& object);
    LightDesc    parseLight(const jsonValue& light);
    WorldDesc    parseWorld(const char* file);
}