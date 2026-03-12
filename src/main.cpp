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
#include "random.hpp"
#include "materials/materials.hpp"
#include "bvhNode.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "imageTexture.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include <atomic>
#include <algorithm>

constexpr uint32 IMG_W = 1200;
constexpr uint32 IMG_H = 675;
constexpr float32 ASPECT_RATIO = static_cast<float32>(IMG_W) / IMG_H;

constexpr uint32  SAMPLES_PER_PIXEL = 500;
constexpr float32 SAMPLE_SCALE      = 1.0f / SAMPLES_PER_PIXEL;
constexpr uint32 MAX_RAY_BOUNCES = 50;

constexpr Vec3 worldY = {0.0f, 1.0f, 0.0f};

const uint32 THREADS = Utility::max(1u, std::thread::hardware_concurrency());

std::vector<Vec3>  framebuffer(IMG_W * IMG_H);
std::vector<uint8> snapshot(IMG_W * IMG_H * 3);

std::vector<std::unique_ptr<Object>> objects;
std::vector<std::unique_ptr<BVHNode>> bvh;
std::vector<std::shared_ptr<Image>> images;
std::vector<std::shared_ptr<Texture>> textures;

Vec3 gammaCorrection(const Vec3& linearColor) {
    return {
        ((linearColor.x > 0.0f) ? Math::sqrt(linearColor.x) : 0.0f),
        ((linearColor.y > 0.0f) ? Math::sqrt(linearColor.y) : 0.0f),
        ((linearColor.z > 0.0f) ? Math::sqrt(linearColor.z) : 0.0f)
    };
}

uint32 longestAxis(const AABB& aabb) {
    const float32 xLength = aabb.max.x - aabb.min.x;
    const float32 yLength = aabb.max.y - aabb.min.y;
    const float32 zLength = aabb.max.z - aabb.min.z;

    if (xLength > yLength) return ((xLength > zLength) ? 0 : 2);
    else                   return ((yLength > zLength) ? 1 : 2);
}
// [from, to)
AABB buildAABB(const std::vector<Object*>& objRefs, const uint32 from, const uint32 to) {
    AABB result;

    for (uint32 i = from; i < to; ++i)
        result = AABB::unite(result, objRefs[i]->aabb());

    return result;
}
// [from, to)
BVHNode* buildBVH(std::vector<Object*>& objRefs, const uint32 from, const uint32 to) {
    bvh.emplace_back(std::make_unique<BVHNode>());

    BVHNode* newNode = bvh.back().get();
    newNode->aabb = buildAABB(objRefs, from, to);

    const uint32 nodeCount = to - from;

    if (nodeCount == 1) {
        newNode->object = objRefs[from];
        return newNode;
    }

    const uint32 axis = longestAxis(newNode->aabb);
    const uint32 midIdx = from + (nodeCount >> 1);

    std::sort(
        objRefs.begin() + from, objRefs.begin() + to,
        [axis](const Object* a, const Object* b) noexcept -> bool {
            return (a->aabb().min.data[axis] < b->aabb().min.data[axis]);
        }
    );

    newNode->left  = buildBVH(objRefs, from, midIdx);
    newNode->right = buildBVH(objRefs, midIdx, to);

    return newNode;
}
void buildBVH() {
    std::vector<Object*> objRefs;
    objRefs.reserve(objects.size());

    for (const auto& obj: objects)
        objRefs.push_back(obj.get());

    buildBVH(objRefs, 0, static_cast<uint32>(objRefs.size()));
}
HitResult traceBVH(const Ray& ray, const float32 tMin, const float32 tMax) { return bvh[0]->hitTest(ray, tMin, tMax); }

Vec3 missShader(const Ray& ray) {
    // move range from [-1, 1] to [0, 1]
    float32 t = 0.5f * (ray.direction.normalize().y + 1.0f);

    // White & SkyBlue
    return Vec3::lerp({1.0f, 1.0f, 1.0f}, {0.5f, 0.7f, 1.0f}, t);
}
Vec3 hitShader(const Ray& ray, const HitResult& hitResult, uint16 depth = 1) {
    if (depth > MAX_RAY_BOUNCES)
        return {};

    SampleResult sample = hitResult.material->sample(ray, hitResult);

    HitResult bounceHitResult = traceBVH(sample.wo, RAY_T_MIN, INF_F32);
    if (bounceHitResult.isHit)
        return sample.albedo * hitShader(sample.wo, bounceHitResult, depth + 1);

    return missShader(sample.wo) * sample.albedo;
}

int main() {
    Camera camera;
    camera.yfov = 20.0f;
    camera.pos  = {0.0f, 1.0f, 3.0f};

    Vec3 nz = camera.pos - camera.look;
    float32 focalLength = nz.length();

    Viewport viewport;
    viewport.height = 2 * focalLength * Math::tan(camera.yfov / 2);
    viewport.width  = viewport.height * ASPECT_RATIO;

    Vec3 z = nz.normalize();
    Vec3 x = Vec3::cross(worldY, z).normalize();
    Vec3 y = Vec3::cross(z, x);

    Vec3 viewportU = x * viewport.width;
    Vec3 viewportV = y * -viewport.height;

    Vec3 viewportdU = viewportU / IMG_W;
    Vec3 viewportdV = viewportV / IMG_H;
    Vec3 viewportLT = camera.pos - (viewportU / 2.0f) - (viewportV / 2.0f) - nz;

    // load textures
    const char* imgFiles[] = {
        "../../resources/textures/earth.jpg"
    };

    for (const auto& file: imgFiles) {
        auto image = std::make_shared<Image>();

        if (!image->load(file)) {
            std::cout << "Failed to load image: " << file << '\n';
            return 0;
        }

        images.emplace_back(image);
    }

    textures.push_back(std::make_shared<ImageTexture>(images[0]));

    auto earth_material = std::make_shared<Lambertian>(textures[0]);
    objects.push_back(std::make_unique<Sphere>(Vec3{0.0f, 0.0f, 0.0f}, 0.5f, earth_material));

    buildBVH();

    std::vector<std::thread> threads;
    threads.reserve(THREADS);

    std::chrono::steady_clock::time_point start, end;
    start = std::chrono::steady_clock::now();
    {
        std::atomic<uint32> nextChunk{};

        auto render = [&]() {
            while (true) {
                uint32 yFrom = nextChunk.fetch_add(1, std::memory_order_relaxed);
                uint32 yTo   = Utility::min(yFrom + 1, IMG_H);

                if (yFrom >= IMG_H)
                    break;

                for (uint32 y = yFrom; y < yTo; ++y) {
                    for (uint32 x = 0; x < IMG_W; ++x) {
                        Vec3 color;

                        for (uint16 sample = 0; sample < SAMPLES_PER_PIXEL; ++sample) {
                            Vec3 jitter = (viewportdU * (x + Random::uniform())) + (viewportdV * (y + Random::uniform()));
                            Vec3 samplePos = viewportLT + jitter;
                            Ray  ray       = {camera.pos, samplePos - camera.pos};

                            // primary ray
                            HitResult hitResult = traceBVH(ray, RAY_T_MIN, INF_F32);
                            color += (hitResult.isHit ? hitShader(ray, hitResult) : missShader(ray));
                        }
                        color *= SAMPLE_SCALE;

                        uint32 idx = (y * IMG_W + x);

                        framebuffer[idx] = color;
                        color = gammaCorrection(color);

                        snapshot[idx * 3    ] = static_cast<uint8>(255.999f * color.r);
                        snapshot[idx * 3 + 1] = static_cast<uint8>(255.999f * color.g);
                        snapshot[idx * 3 + 2] = static_cast<uint8>(255.999f * color.b);
                    }
                }
            }
        };

        for (uint16 i = 0; i < THREADS; ++i)
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