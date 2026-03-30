#pragma once

#include "components/transform.hpp"

#include <string>
#include <variant>
#include <vector>

struct RenderSettings {
    u32 imgWidth {};
    u32 imgHeight{};

    u32 samplesPerPixel{};
    u32 maxRayBounces  {};
};

enum class BxDFType: byte {
    None       = 0x00,
    Lambertian = 0x01,
    Metal      = 0x02,
    Dielectric = 0x03,
    Unknown    = 0xFF
};
enum class TextureType: byte {
    None    = 0x00,
    Solid   = 0x01,
    Image   = 0x02,
    Unknown = 0xFF
};
enum class GeometryType: byte {
    None    = 0x00,
    Sphere  = 0x01,
    Quad    = 0x02,
    Box     = 0x03,
    Unknown = 0xFF
};

struct MetalDesc      { f32 roughness      {}; };
struct DielectricDesc { f32 refractiveIndex{}; };

struct SolidTextureDesc { Vec3        color; };
struct ImageTextureDesc { std::string file;  };

struct MaterialDesc {
    BxDFType    bxdfType;
    TextureType textureType;

    std::variant<MetalDesc, DielectricDesc>          bxdfData;
    std::variant<SolidTextureDesc, ImageTextureDesc> textureData;

    std::string name;
};

struct SphereDesc { f32  radius{}; };
struct QuadDesc   { Vec3 u ,  v;   };
struct BoxDesc    { Vec3 p1, p2;   };
struct ObjectDesc {
    GeometryType geometryType;

    std::variant<SphereDesc, QuadDesc, BoxDesc> geometryData;

    Transform transform;

    std::string name;
    std::string materialName;
};

struct LightDesc {
    Vec3 color;
    f32  intensity{};

    std::string objectName;
};

struct WorldDesc {
    // std::string name;

    std::vector<MaterialDesc> materials;
    std::vector<ObjectDesc>   objects;
    std::vector<LightDesc>    lights;
};