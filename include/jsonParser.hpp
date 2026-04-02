#pragma once

#include "components/transform.hpp"

#include <string>

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
    template <typename Encoding, typename Allocator, typename StackAllocator>
    class GenericDocument;
}
using jsonValue    = rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>;
using jsonDocument = rapidjson::GenericDocument<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>, rapidjson::CrtAllocator>;

namespace JsonParser {
    jsonDocument getDocument(const char* file);

    u32 toU32(const jsonValue& v);
    f32 toF32(const jsonValue& v);

    std::string toString(const jsonValue& v);

    Vec3      toVec3(const jsonValue& v);
    Transform toTransform(const jsonValue& v);
}