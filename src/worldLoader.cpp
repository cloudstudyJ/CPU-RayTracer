#include "worldLoader.hpp"

#include "rapidjson/document.h"

namespace {
    BxDFType toBxDFType(const std::string& str) {
        if      (str == "Lambertian") return BxDFType::Lambertian;
        else if (str == "Metal"     ) return BxDFType::Metal;
        else if (str == "Dielectric") return BxDFType::Dielectric;

        return BxDFType::Unknown;
    }
    TextureType toTextureType(const std::string& str) {
        if      (str == "Solid") return TextureType::Solid;
        else if (str == "Image") return TextureType::Image;

        return TextureType::Unknown;
    }
    GeometryType toGeometryType(const std::string& str) {
        if      (str == "Sphere") return GeometryType::Sphere;
        else if (str == "Quad"  ) return GeometryType::Quad;
        else if (str == "Box"   ) return GeometryType::Box;

        return GeometryType::Unknown;
    }
}

namespace WorldLoader {
    MaterialDesc parseMaterial(const jsonValue& material) {
        using namespace JsonParser;

        const jsonValue& bxdf    = material["BxDF"];
        const jsonValue& texture = material["Texture"];

        MaterialDesc desc = {
            .bxdfType    = toBxDFType(toString(bxdf["Type"])),
            .textureType = toTextureType(toString(texture["Type"])),
            .name        = toString(material["Name"]),
        };

        if      (desc.bxdfType == BxDFType::Metal     ) desc.bxdfData = MetalDesc{toF32(bxdf["Roughness"])};
        else if (desc.bxdfType == BxDFType::Dielectric) desc.bxdfData = DielectricDesc{toF32(bxdf["RefractiveIndex"])};

        if      (desc.textureType == TextureType::Solid) desc.textureData = SolidTextureDesc{toVec3(texture["Color"])};
        else if (desc.textureType == TextureType::Image) desc.textureData = ImageTextureDesc{toString(texture["File"])};

        return desc;
    }
    ObjectDesc parseObject(const jsonValue& object) {
        using namespace JsonParser;

        const jsonValue& geo = object["Geometry"];

        ObjectDesc desc = {
            .geometryType = toGeometryType(toString(geo["Type"])),
            .transform    = toTransform(object["Transform"]),
            .name         = toString(object["Name"])
        };

        if      (desc.geometryType == GeometryType::Sphere) desc.geometryData = SphereDesc{toF32(geo["Radius"])};
        else if (desc.geometryType == GeometryType::Quad  ) desc.geometryData = QuadDesc{toVec3(geo["U"]), toVec3(geo["V"])};
        else if (desc.geometryType == GeometryType::Box   ) desc.geometryData = BoxDesc{toVec3(geo["P1"]), toVec3(geo["P2"])};

        if (object.HasMember("Material"))
            desc.materialName = toString(object["Material"]);

        return desc;
    }
    LightDesc parseLight(const jsonValue& light) {
        using namespace JsonParser;

        return {
            .color      = toVec3(light["Color"]),
            .intensity  = toF32(light["Intensity"]),
            .objectName = toString(light["Object"])
        };
    }
    WorldDesc parseWorld(const char* file) {
        jsonDocument doc = JsonParser::getDocument(file);

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