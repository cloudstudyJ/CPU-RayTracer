#include "types.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "viewport.hpp"
#include "math.hpp"
#include "sphere.hpp"
#include "solver.hpp"

#include <fstream>

constexpr uint16 IMG_W = 400;
constexpr uint16 IMG_H = 225;
constexpr float32 ASPECT_RATIO = static_cast<float32>(IMG_W) / IMG_H;

Vec3 missShader(const Ray& ray) {
    // range from [-1, 1] to [0, 1]
    float32 t = 0.5f * (Math::normalize(ray.direction()).y + 1.0f);

    // White & SkyBlue
    return Math::lerp(Vec3{1.0f, 1.0f, 1.0f}, Vec3{0.5f, 0.7f, 1.0f}, t);
}
Vec3 hitShader(const Ray& ray, const HitResult& hitResult) {
    float32 closestT = ((hitResult.tIn > 0.001f) ? hitResult.tIn : hitResult.tOut);
    Vec3 normal = Math::normalize(ray.at(closestT) - hitResult.object->centerPos()) + 1;

    return (normal * 0.5f);
}

void writeHeader(std::ofstream& writer) {
    writer << "P3\n";
    writer << IMG_W << ' ' << IMG_H << '\n';
    writer << 255 << '\n';
}
void drawPixel(std::ofstream& writer, const Vec3& color) {
    // convert to 8-bits linear color space [0, 255]
    uint8 iR = static_cast<uint8>(255.999f * color.r);
    uint8 iG = static_cast<uint8>(255.999f * color.g);
    uint8 iB = static_cast<uint8>(255.999f * color.b);

    writer << static_cast<int32>(iR) << ' '
           << static_cast<int32>(iG) << ' '
           << static_cast<int32>(iB) << '\n';
}

int main() {
    Camera camera;
    Viewport viewport;

    viewport.height = 2 * viewport.focalLength * Math::tan(camera.yFOV / 2);
    viewport.width  = viewport.height * ASPECT_RATIO;

    Vec3 dUV = Vec3{viewport.width / IMG_W, (-viewport.height) / IMG_H, 0.0f};
    Vec3 viewportOrigin = camera.pos + Vec3{(-viewport.width) / 2, viewport.height / 2, -viewport.focalLength};
    Vec3 pixelOriginCenter = viewportOrigin + (dUV * 0.5f);

    Sphere sphere(Vec3{0.0f, 0.0f, -1.0f}, 0.5f);

    {
        // render
        std::ofstream writer("result.ppm");
        writeHeader(writer);

        for (uint16 y = 0; y < IMG_H; ++y) {
            for (uint16 x = 0; x < IMG_W; ++x) {
                Vec3 pixelCenter = pixelOriginCenter + Vec3{x * dUV.x, y * dUV.y, 0.0f};
                Vec3 rayDir = pixelCenter - camera.pos;

                Ray ray(camera.pos, rayDir);

                HitResult hitResult = Solver::hitTest(ray, sphere);
                Vec3 color = (hitResult.isHit ? hitShader(ray, hitResult) : missShader(ray));

                drawPixel(writer, color);
            }
        }
    }

    return 0;
}