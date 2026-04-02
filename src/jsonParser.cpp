#include "jsonParser.hpp"

#include "rapidjson/document.h"

#include <iostream>
#include <fstream>
#include <vector>

namespace {
    std::vector<byte> readFile(const char* file) {
        std::vector<byte> data;
        std::ifstream reader(file, std::ios::binary | std::ios::ate);

        if (reader) {
            u64 fileSize = static_cast<u64>(reader.tellg());
            reader.seekg(0, std::ios::beg);

            data.resize(fileSize, 0);
            reader.read(reinterpret_cast<char*>(data.data()), fileSize);
        }
        else
            std::cout << "Failed to open file: \"" << file << "\"\n";

        return data;
    }
}

namespace JsonParser {
    jsonDocument getDocument(const char* file) {
        std::vector<byte> json = readFile(file);

        jsonDocument doc;
        doc.Parse(reinterpret_cast<const char*>(json.data()), json.size());

        return doc;
    }

    u32 toU32(const jsonValue& v) { return v.GetUint();  }
    f32 toF32(const jsonValue& v) { return v.GetFloat(); }

    std::string toString(const jsonValue& v) { return v.GetString(); }

    Vec3      toVec3(const jsonValue& v)      { return {          toF32(v[0]),           toF32(v[1]),        toF32(v[2])}; }
    Transform toTransform(const jsonValue& v) { return {toVec3(v["Position"]), toVec3(v["Rotation"]), toVec3(v["Scale"])}; }
}