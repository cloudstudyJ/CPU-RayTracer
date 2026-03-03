#include <fstream>

#include "types.hpp"

constexpr uint16 IMG_W = 1024;
constexpr uint16 IMG_H = 1024;

int main() {
    std::ofstream writer("result.ppm");

    writer << "P3\n";
    writer << IMG_W << ' ' << IMG_H << '\n';
    writer << 255 << '\n';

    for (uint16 y = 0; y < IMG_H; ++y) {
        for (uint16 x = 0; x < IMG_W; ++x) {
            // r, g, b = [0.0f, 1.0f]
            float32 r = static_cast<float32>(x) / (IMG_W - 1);
            float32 g = static_cast<float32>(y) / (IMG_H - 1);
            float32 b = 0.0f;

            // convert to 8-bits linear color space [0, 255]
            uint8 iR = static_cast<uint8>(255.999f * r);
            uint8 iG = static_cast<uint8>(255.999f * g);
            uint8 iB = static_cast<uint8>(255.999f * b);

            writer << static_cast<int>(iR) << ' '
                   << static_cast<int>(iG) << ' '
                   << static_cast<int>(iB) << '\n';
        }
    }

    return 0;
}