#pragma once

#include "components/camera.hpp"
#include "components/transform.hpp"

#include <string>
#include <variant>
#include <vector>

enum class BxDFType: byte {
    None       = 0,
    Lambertian = 1 << 0,
    Metal      = 1 << 1,
    Dielectric = 1 << 2,
    Unknown    = 0xFF
};
enum class TextureSource: byte {
    None     = 0,
    Constant = 1 << 0,
    Image    = 1 << 1,
    Unknown  = 0xFF
};
enum class TextureType: byte {
    None      = 0,
    BaseColor = 1 << 0,
    Normal    = 1 << 1,
    Roughness = 1 << 2,
    Metallic  = 1 << 3,
    // Occlusion + Roughness + Metallic
    ORM       = Roughness | Metallic,
    Unknown   = 0xFF
};
enum class GeometryType: byte {
    None      = 0,
    Sphere    = 1 << 0,
    Rectangle = 1 << 1,
    Cube      = 1 << 2,
    Unknown   = 0xFF
};

struct MetalDesc {
    f32 metallic { 1.0f };
    f32 roughness{ 1.0f };
};
struct DielectricDesc {
    f32 ior{ 1.0f };
};

struct ConstantTextureDesc {
    Vec3 color;
};
struct ImageTextureDesc {
    std::string file;
};
struct TextureDesc {
    TextureType   type;
    TextureSource src;

    std::variant<ConstantTextureDesc, ImageTextureDesc> data;
};

struct MaterialDesc {
    BxDFType bxdfType;

    std::variant<MetalDesc, DielectricDesc> bxdfDesc;
    std::vector<TextureDesc>                textures;

    std::string name;
};

struct SphereDesc {
    f32 radius{ 1.0f };
};
struct ObjectDesc {
    GeometryType geometryType;

    std::variant<SphereDesc> geometryData;

    Transform transform;

    std::string name;
    std::string materialName;
};

struct LightDesc {
    Vec3 color;
    f32  intensity{ 1.0f };

    std::string objectName;
};

struct WorldDesc {
    // std::string name;
    Camera camera;

    std::vector<MaterialDesc> materials;
    std::vector<ObjectDesc>   objects;
    std::vector<LightDesc>    lights;
};