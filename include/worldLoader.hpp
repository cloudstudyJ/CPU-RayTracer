#pragma once

#include "worldTypes.hpp"

// fwd
namespace rapidjson {
    // encodings
    template<typename CharType> struct UTF8;

    // allocators
    class CrtAllocator;
    template <typename BaseAllocator>
    class MemoryPoolAllocator;

    // documents
    template <typename Encoding, typename Allocator>
    class GenericValue;
}
using jsonValue = rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>;

namespace WorldLoader {
    MaterialDesc parseMaterial(const jsonValue& material);
    ObjectDesc   parseObject(const jsonValue& object);
    LightDesc    parseLight(const jsonValue& light);

    WorldDesc parseWorld(const char* file);
}