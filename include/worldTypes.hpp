#pragma once

#include "components/camera.hpp"
#include "components/transform.hpp"

#include <string>
#include <variant>
#include <vector>

enum class BxDFType: byte {
    None       = 0x00,
    Lambertian = 0x01,
    Metal      = 0x02,
    Dielectric = 0x03,
    Unknown    = 0xFF
};
enum class TextureData: byte {
    None     = 0x00,
    Constant = 0x01,
    Image    = 0x02,
    Unknown  = 0xFF
};
enum class GeometryType: byte {
    None      = 0x00,
    Sphere    = 0x01,
    Rectangle = 0x02,
    Cube      = 0x03,
    Unknown   = 0xFF
};

struct MetalDesc      { f32 roughness      {}; };
struct DielectricDesc { f32 refractiveIndex{}; };

struct SolidTextureDesc { Vec3        color; };
struct ImageTextureDesc { std::string file;  };

struct MaterialDesc {
    BxDFType    bxdfType;
    TextureData textureData;

    std::variant<MetalDesc, DielectricDesc>          bxdfDesc;
    std::variant<SolidTextureDesc, ImageTextureDesc> textureDesc;

    std::string name;
};

struct SphereDesc { f32  radius{}; };
struct ObjectDesc {
    GeometryType geometryType;

    std::variant<SphereDesc> geometryData;

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
    Camera camera;

    std::vector<MaterialDesc> materials;
    std::vector<ObjectDesc>   objects;
    std::vector<LightDesc>    lights;
};