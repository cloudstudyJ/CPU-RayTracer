#include "types.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "viewport.hpp"
#include "math.hpp"
#include "sphere.hpp"
#include "hitResult.hpp"
#include "constants.hpp"
#include "utility.hpp"

#include <fstream>
#include <vector>
#include <memory>

constexpr uint16 IMG_W = 400;
constexpr uint16 IMG_H = 225;
constexpr float32 ASPECT_RATIO = static_cast<float32>(IMG_W) / IMG_H;

constexpr uint16  SAMPLES_PER_PIXEL = 64;
constexpr float32 SAMPLE_SCALE      = 1.0f / SAMPLES_PER_PIXEL;

Vec3 missShader(const Ray& ray) {
    // move range from [-1, 1] to [0, 1]
    float32 t = 0.5f * (Math::normalize(ray.direction()).y + 1.0f);

    // White & SkyBlue
    return Math::lerp(Vec3{1.0f, 1.0f, 1.0f}, Vec3{0.5f, 0.7f, 1.0f}, t);
}
Vec3 hitShader(const Ray& ray, const HitResult& hitResult) { return ((hitResult.normal + 1) * 0.5f); }

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
    Vec3 viewportLT = camera.pos + Vec3{(-viewport.width) / 2, viewport.height / 2, -viewport.focalLength};
    Vec3 pixelOriginCenter = viewportLT + (dUV * 0.5f);

    std::vector<std::unique_ptr<Object>> objects;
    objects.push_back(std::make_unique<Sphere>(Vec3{0.0f, 0.0f, -1.0f}, 0.5f));
    objects.push_back(std::make_unique<Sphere>(Vec3{0.0f, -100.5f, -1.0f}, 100.0f));

    {
        // render
        std::ofstream writer("result.ppm");
        writeHeader(writer);

        for (uint16 y = 0; y < IMG_H; ++y) {
            for (uint16 x = 0; x < IMG_W; ++x) {
                Vec3 color;

                for (uint16 sample = 0; sample < SAMPLES_PER_PIXEL; ++sample) {
                    Vec3 jitter = {(x + Utility::random()) * dUV.x, (y + Utility::random()) * dUV.y, 0.0f};
                    Vec3 samplePos = viewportLT + jitter;
                    Ray ray(camera.pos, samplePos - camera.pos);

                    HitResult hitResult;
                    for (const auto& object: objects) {
                        HitResult currentHitResult = object->hitTest(ray, RAY_T_MIN, hitResult.hitT);

                        if (currentHitResult.isHit && (currentHitResult.hitT < hitResult.hitT))
                            hitResult = currentHitResult;
                    }

                    color += (hitResult.isHit ? hitShader(ray, hitResult) : missShader(ray));
                }

                drawPixel(writer, color * SAMPLE_SCALE);
            }
        }
    }

    return 0;
}