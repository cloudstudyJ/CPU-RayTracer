#include "worldBuilder.hpp"
#include "render/renderer.hpp"

#include <iostream>
#include <vector>
#include <chrono>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

std::chrono::steady_clock::time_point start, end;

Vec3 gammaCorrection(const Vec3& linearColor) {
    return {
        Math::sqrt(Utility::clamp(linearColor.r, 0.0f, 1.0f)),
        Math::sqrt(Utility::clamp(linearColor.g, 0.0f, 1.0f)),
        Math::sqrt(Utility::clamp(linearColor.b, 0.0f, 1.0f))
    };
}

bool saveScreenshot(const auto& renderConfig, const std::vector<byte>& snapshot, const char* filename) {
    const i32 w = static_cast<i32>(renderConfig.imgW);
    const i32 h = static_cast<i32>(renderConfig.imgH);

    return stbi_write_png(filename, w, h, 3, snapshot.data(), w * 3) != 0;
}

int main() {
    Renderer renderer("./renderConfig.json");
    const auto renderConfig = renderer.renderConfig();

    World world = std::move(WorldBuilder::buildWorld("./resources/worlds/cornellbox.json"));

    start = std::chrono::steady_clock::now();
    renderer.render(world);
    end = std::chrono::steady_clock::now();

    const auto& frameBuffer = renderer.frameBuffer();
    std::vector<byte> snapshot;
    snapshot.reserve(frameBuffer.size() * 3);

    for (auto color: frameBuffer) {
        color = gammaCorrection(color);

        snapshot.push_back(static_cast<byte>(255.999f * color.r));
        snapshot.push_back(static_cast<byte>(255.999f * color.g));
        snapshot.push_back(static_cast<byte>(255.999f * color.b));
    }
    saveScreenshot(renderConfig, snapshot, "result.png");

    std::cout << "Threads: " << renderConfig.threads << '\n';
    std::cout << "Resolution: " << renderConfig.imgW << 'x' << renderConfig.imgH << '\n';
    std::cout << "Max Ray Bounces: " << renderConfig.maxRayDepth << '\n';
    std::cout << "Samples Per Pixel: " << renderConfig.spp << '\n';
    std::cout << "Render Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " [ms]\n";

    return 0;
}