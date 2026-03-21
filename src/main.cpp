#include "ray.hpp"
#include "camera.hpp"
#include "math.hpp"
#include "hitResult.hpp"
#include "utility.hpp"
#include "random.hpp"
#include "material.hpp"
#include "solidTexture.hpp"
#include "objects.hpp"
#include "bxdfs.hpp"
#include "world.hpp"
#include "vec2.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include <atomic>

constexpr u32 IMG_W = 600;
constexpr u32 IMG_H = 600;
constexpr f32 ASPECT_RATIO = static_cast<f32>(IMG_W) / IMG_H;

constexpr u32 SAMPLES_PER_PIXEL = 200;
constexpr f32 SAMPLE_SCALE      = 1.0f / SAMPLES_PER_PIXEL;
constexpr u32 MAX_RAY_BOUNCES   = 50;

constexpr Vec3 WORLD_Y     = {0.0f, 1.0f, 0.0f};
constexpr Vec3 COLOR_BLACK = {0.0f, 0.0f, 0.0f};
constexpr Vec3 COLOR_WHITE = {1.0f, 1.0f, 1.0f};

const u32 THREADS        = Utility::max(1u, std::thread::hardware_concurrency());
const u16 SQRT_SPP       = static_cast<u16>(Math::sqrt(SAMPLES_PER_PIXEL));
const f32 INV_SQRT_SPP   = 1.0f / SQRT_SPP;
const f32 SQRT_SPP_SCALE = 1.0f / (SQRT_SPP * SQRT_SPP);

std::vector<Vec3> framebuffer(IMG_W * IMG_H);
std::vector<byte> snapshot(IMG_W * IMG_H * 3);

std::vector<std::shared_ptr<BxDF>>     bxdfs;
std::vector<std::shared_ptr<Texture>>  textures;
std::vector<std::shared_ptr<Material>> materials;

World world;

Vec3 gammaCorrection(const Vec3& linearColor) {
    return {
        ((linearColor.x > 0.0f) ? Math::sqrt(linearColor.x) : 0.0f),
        ((linearColor.y > 0.0f) ? Math::sqrt(linearColor.y) : 0.0f),
        ((linearColor.z > 0.0f) ? Math::sqrt(linearColor.z) : 0.0f)
    };
}

// Black Backround
Vec3 missShader(const Ray& ray) { return COLOR_BLACK; }
Vec3 hitShader(const Ray& ray, const HitResult& hitResult, u16 depth = 1) {
    if (depth > MAX_RAY_BOUNCES)
        return {};

    // if object is light
    if (!hitResult.material->bxdf)
        return (hitResult.material->emitColor * hitResult.material->emitIntensity);

    Vec3 wo = hitResult.material->bxdf->sample(ray.direction, hitResult);

    HitResult bounceHitResult = world.traceRay(Ray{hitResult.hitPoint, wo}, RAY_T_MIN, INF_F32);
    if (bounceHitResult.isHit)
        return hitResult.material->texture->color(hitResult.u, hitResult.v) * hitShader(Ray{hitResult.hitPoint, wo}, bounceHitResult, depth + 1);

    return missShader(Ray{hitResult.hitPoint, wo}) * hitResult.material->texture->color(hitResult.u, hitResult.v);
}

void makeCornellBox() {
    bxdfs.push_back(std::make_shared<Lambertian>());

    textures.push_back(std::make_shared<SolidTexture>(Vec3{0.12f, 0.45f, 0.15f}));  // green
    textures.push_back(std::make_shared<SolidTexture>(Vec3{0.65f, 0.05f, 0.05f}));  // red
    textures.push_back(std::make_shared<SolidTexture>(Vec3{0.73f, 0.73f, 0.73f}));  // white
    textures.push_back(std::make_shared<SolidTexture>(Vec3{1.0f, 1.0f, 1.0f}));     // pure white

    materials.push_back(std::make_shared<Material>(Material{bxdfs[0].get(), textures[0].get()}));                           // green wall (Lambertian)
    materials.push_back(std::make_shared<Material>(Material{bxdfs[0].get(), textures[1].get()}));                           // red   wall (Lambertian)
    materials.push_back(std::make_shared<Material>(Material{bxdfs[0].get(), textures[2].get()}));                           // white wall (Lambertian)
    materials.push_back(std::make_shared<Material>(Material{nullptr, textures[3].get(), COLOR_WHITE, 15.0f}));   // light

    world.addObject<Quad>(Vec3{555.0f, 0.0f, 0.0f}, Vec3{0.0f, 555.0f, 0.0f}, Vec3{0.0f, 0.0f, 555.0f}, materials[0].get());
    world.addObject<Quad>(Vec3{0.0f, 0.0f, 0.0f}, Vec3{0.0f, 555.0f, 0.0f}, Vec3{0.0f, 0.0f, 555.0f}, materials[1].get());
    world.addObject<Quad>(Vec3{0.0f, 0.0f, 0.0f}, Vec3{555.0f, 0.0f, 0.0f}, Vec3{0.0f, 0.0f, 555.0f}, materials[2].get());
    world.addObject<Quad>(Vec3{555.0f, 555.0f, 555.0f}, Vec3{-555.0f, 0.0f, 0.0f}, Vec3{0.0f, 0.0f, -555.0f}, materials[2].get());
    world.addObject<Quad>(Vec3{0.0f, 0.0f, 555.0f}, Vec3{555.0f, 0.0f, 0.0f}, Vec3{0.0f, 555.0f, 0.0f}, materials[2].get());
    world.addObject<Quad>(Vec3{343.0f, 554.0f, 332.0f}, Vec3{-130.0f, 0.0f, 0.0f}, Vec3{0.0f, 0.0f, -105.0f}, materials[3].get());
}

int main() {
    Camera camera = {
        .yfov = 40.0f,
        .pos  = {278.0f, 278.0f, -800.0f},
        .look = {278.0f, 278.0f, 0.0f}
    };

    Vec3 nz = camera.pos - camera.look;
    f32 focalLength = nz.length();

    Vec2 viewport;
    viewport.y = 2 * focalLength * Math::tan(Math::toRad(camera.yfov / 2));
    viewport.x = viewport.y * ASPECT_RATIO;

    Vec3 z = nz.normalize();
    Vec3 x = Vec3::cross(WORLD_Y, z).normalize();
    Vec3 y = Vec3::cross(z, x);

    Vec3 viewportU = x * viewport.x;
    Vec3 viewportV = y * -viewport.y;

    Vec3 viewportdU = viewportU / IMG_W;
    Vec3 viewportdV = viewportV / IMG_H;
    Vec3 viewportLT = camera.pos - (viewportU / 2.0f) - (viewportV / 2.0f) - nz;

    makeCornellBox();
    world.addObject<Box>(Vec3{130.0f, 0.0f, 65.0f}, Vec3{295.0f, 165.0f, 230.0f}, materials[2].get());
    world.addObject<Box>(Vec3{265.0f, 0.0f, 295.0f}, Vec3{430.0f, 330.0f, 460.0f}, materials[2].get());
    world.buildAccelerationStructure();

    std::vector<std::thread> threads;
    threads.reserve(THREADS);

    std::chrono::steady_clock::time_point start, end;
    start = std::chrono::steady_clock::now();
    {
        std::atomic<u32> nextChunk{};

        auto render = [&]() {
            while (true) {
                u32 yFrom = nextChunk.fetch_add(1, std::memory_order_relaxed);
                u32 yTo   = Utility::min(yFrom + 1, IMG_H);

                if (yFrom >= IMG_H)
                    break;

                for (u32 y = yFrom; y < yTo; ++y) {
                    for (u32 x = 0; x < IMG_W; ++x) {
                        Vec3 color;

                        for (u16 sampleY = 0; sampleY < SQRT_SPP; ++sampleY) {
                            for (u16 sampleX = 0; sampleX < SQRT_SPP; ++sampleX) {
                                f32 offsetX = x + ((sampleX + Random::uniform()) * INV_SQRT_SPP);
                                f32 offsetY = y + ((sampleY + Random::uniform()) * INV_SQRT_SPP);

                                Vec3 samplePos = viewportLT + (viewportdU * offsetX) + (viewportdV * offsetY);
                                Ray  ray       = {camera.pos, samplePos - camera.pos};

                                HitResult hitResult = world.traceRay(ray, RAY_T_MIN, INF_F32);
                                color += (hitResult.isHit ? hitShader(ray, hitResult) : missShader(ray));
                            }
                        }
                        color *= SQRT_SPP_SCALE;

                        u32 idx = (y * IMG_W + x);

                        framebuffer[idx] = color;
                        color = gammaCorrection(color);
                        color.r = Utility::clamp(color.x, 0.0f, 1.0f);
                        color.g = Utility::clamp(color.y, 0.0f, 1.0f);
                        color.b = Utility::clamp(color.z, 0.0f, 1.0f);

                        snapshot[idx * 3    ] = static_cast<byte>(255.999f * color.r);
                        snapshot[idx * 3 + 1] = static_cast<byte>(255.999f * color.g);
                        snapshot[idx * 3 + 2] = static_cast<byte>(255.999f * color.b);
                    }
                }
            }
        };

        for (u16 i = 0; i < THREADS; ++i)
            threads.emplace_back(render);

        for (auto& thread: threads)
            thread.join();
    }
    end = std::chrono::steady_clock::now();

    std::ofstream writer("result.ppm", std::ios::binary);

    writer << "P6\n";
    writer << IMG_W << ' ' << IMG_H << '\n';
    writer << "255\n";
    writer.write(reinterpret_cast<const char*>(snapshot.data()), snapshot.size());

    std::cout << "Threads: " << THREADS << '\n';
    std::cout << "Resolution: " << IMG_W << 'x' << IMG_H << '\n';
    std::cout << "Max Ray Bounces: " << MAX_RAY_BOUNCES << '\n';
    std::cout << "Samples Per Pixel: " << SAMPLES_PER_PIXEL << '\n';
    std::cout << "Render Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " [ms]\n";

    return 0;
}