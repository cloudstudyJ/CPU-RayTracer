#include "worldBuilder.hpp"
#include "render/renderer.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

World world;

std::chrono::steady_clock::time_point start, end;

Vec3 gammaCorrection(const Vec3& linearColor) {
    return {
        .data = {
            ((linearColor.x > 0.0f) ? Math::sqrt(Utility::max(0.0f, linearColor.x)) : 0.0f),
            ((linearColor.y > 0.0f) ? Math::sqrt(Utility::max(0.0f, linearColor.y)) : 0.0f),
            ((linearColor.z > 0.0f) ? Math::sqrt(Utility::max(0.0f, linearColor.z)) : 0.0f)
        }
    };
}

int main() {
    Renderer renderer("../../renderConfig.json");
    const auto renderConfig = renderer.renderConfig();

    world = std::move(WorldBuilder::buildWorld("../../resources/worlds/cornellbox.json"));

    start = std::chrono::steady_clock::now();
    renderer.render(world);
    end = std::chrono::steady_clock::now();

    const auto& frameBuffer = renderer.frameBuffer();
    std::vector<byte> snapshot;
    snapshot.reserve(frameBuffer.size() * 3);

    for (auto color: frameBuffer) {
        color = gammaCorrection(color);
        color.r = Utility::clamp(color.x, 0.0f, 1.0f);
        color.g = Utility::clamp(color.y, 0.0f, 1.0f);
        color.b = Utility::clamp(color.z, 0.0f, 1.0f);

        snapshot.push_back(static_cast<byte>(255.999f * color.r));
        snapshot.push_back(static_cast<byte>(255.999f * color.g));
        snapshot.push_back(static_cast<byte>(255.999f * color.b));
    }

    std::ofstream writer("result.ppm", std::ios::binary);

    writer << "P6\n";
    writer << renderConfig.imgW << ' ' << renderConfig.imgH << '\n';
    writer << "255\n";
    writer.write(reinterpret_cast<const char*>(snapshot.data()), snapshot.size());

    std::cout << "Threads: " << renderConfig.threads << '\n';
    std::cout << "Resolution: " << renderConfig.imgW << 'x' << renderConfig.imgH << '\n';
    std::cout << "Max Ray Bounces: " << renderConfig.maxRayDepth << '\n';
    std::cout << "Samples Per Pixel: " << renderConfig.spp << '\n';
    std::cout << "Render Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " [ms]\n";

    return 0;
}