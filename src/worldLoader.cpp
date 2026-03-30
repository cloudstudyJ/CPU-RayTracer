#include "worldLoader.hpp"

#include "rapidjson/document.h"

#include <fstream>
#include <iostream>

using jsonDocument = rapidjson::Document;

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
    u32 toU32(const jsonValue& v) { return v.GetUint();  }
    f32 toF32(const jsonValue& v) { return v.GetFloat(); }

    std::string toString(const jsonValue& v) { return v.GetString(); }

    Vec3      toVec3(const jsonValue& v)      { return {          toF32(v[0]),           toF32(v[1]),        toF32(v[2])}; }
    Transform toTransform(const jsonValue& v) { return {toVec3(v["Position"]), toVec3(v["Rotation"]), toVec3(v["Scale"])}; }

    BxDFType toBxDFType(const jsonValue& v) {
        const std::string str = toString(v);

        if      (str == "Lambertian") return BxDFType::Lambertian;
        else if (str == "Metal"     ) return BxDFType::Metal;
        else if (str == "Dielectric") return BxDFType::Dielectric;

        return BxDFType::Unknown;
    }
    TextureType toTextureType(const jsonValue& v) {
        const std::string str = toString(v);

        if      (str == "Solid") return TextureType::Solid;
        else if (str == "Image") return TextureType::Image;

        return TextureType::Unknown;
    }
    GeometryType toGeometryType(const jsonValue& v) {
        const std::string str = toString(v);

        if      (str == "Sphere") return GeometryType::Sphere;
        else if (str == "Quad"  ) return GeometryType::Quad;
        else if (str == "Box"   ) return GeometryType::Box;

        return GeometryType::Unknown;
    }
}

namespace WorldLoader {
    MaterialDesc parseMaterial(const jsonValue& material) {
        const jsonValue& bxdf    = material["BxDF"];
        const jsonValue& texture = material["Texture"];

        MaterialDesc desc = {
            .bxdfType    = JsonParser::toBxDFType(bxdf["Type"]),
            .textureType = JsonParser::toTextureType(texture["Type"]),
            .name        = JsonParser::toString(material["Name"]),
        };

        if      (desc.bxdfType == BxDFType::Metal     ) desc.bxdfData = MetalDesc{JsonParser::toF32(bxdf["Roughness"])};
        else if (desc.bxdfType == BxDFType::Dielectric) desc.bxdfData = DielectricDesc{JsonParser::toF32(bxdf["RefractiveIndex"])};

        if      (desc.textureType == TextureType::Solid) desc.textureData = SolidTextureDesc{JsonParser::toVec3(texture["Color"])};
        else if (desc.textureType == TextureType::Image) desc.textureData = ImageTextureDesc{JsonParser::toString(texture["File"])};

        return desc;
    }
    ObjectDesc parseObject(const jsonValue& object) {
        const jsonValue& geo = object["Geometry"];

        ObjectDesc desc = {
            .geometryType = JsonParser::toGeometryType(geo["Type"]),
            .transform    = JsonParser::toTransform(object["Transform"]),
            .name         = JsonParser::toString(object["Name"])
        };

        if      (desc.geometryType == GeometryType::Sphere) desc.geometryData = SphereDesc{JsonParser::toF32(geo["Radius"])};
        else if (desc.geometryType == GeometryType::Quad  ) desc.geometryData = QuadDesc{JsonParser::toVec3(geo["U"]), JsonParser::toVec3(geo["V"])};
        else if (desc.geometryType == GeometryType::Box   ) desc.geometryData = BoxDesc{JsonParser::toVec3(geo["P1"]), JsonParser::toVec3(geo["P2"])};

        if (object.HasMember("Material"))
            desc.materialName = JsonParser::toString(object["Material"]);

        return desc;
    }
    LightDesc parseLight(const jsonValue& light) {
        return {
            .color      = JsonParser::toVec3(light["Color"]),
            .intensity  = JsonParser::toF32(light["Intensity"]),
            .objectName = JsonParser::toString(light["Object"])
        };
    }

    WorldDesc parseWorld(const char* file) {
        std::vector<byte> json = readFile(file);

        jsonDocument doc;
        doc.Parse(reinterpret_cast<char*>(json.data()), json.size());

        const jsonValue& materials = doc["Materials"];
        const jsonValue& objects   = doc["Objects"];
        const jsonValue& lights    = doc["Lights"];

        WorldDesc desc;
        for (u32 i = 0; i < materials.Size(); ++i) desc.materials.emplace_back(parseMaterial(materials[i]));
        for (u32 i = 0; i < objects.Size()  ; ++i) desc.objects.emplace_back(parseObject(objects[i]));
        for (u32 i = 0; i < lights.Size()   ; ++i) desc.lights.emplace_back(parseLight(lights[i]));

        return desc;
    }
}