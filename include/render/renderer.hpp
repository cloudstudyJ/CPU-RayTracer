#pragma once

#include "math/vec3.hpp"
#include "integrator/path.hpp"

#include <vector>

// fwd
struct Ray;
struct HitResult;
struct Camera;
struct Viewport;

// fwd
class World;

class Renderer {
    Renderer(const Renderer&)                = delete;
    Renderer(Renderer&&) noexcept            = delete;
    Renderer& operator=(const Renderer&)     = delete;
    Renderer& operator=(Renderer&&) noexcept = delete;

    public:
        struct RenderConfig {
            u32 imgW       {};
            u32 imgH       {};
            f32 aspect     {};
            u32 spp        {};
            u32 maxRayDepth{};
            u32 threads    {};
        };

    public:
        explicit Renderer(const char* renderConfigPath);
        ~Renderer() noexcept = default;

        void render(const World& world);

        const RenderConfig      renderConfig() const noexcept;
        const std::vector<Vec3> frameBuffer() const noexcept;

    private:
        Vec3 renderPixel(const World& world, const Camera& camera, const Viewport& viewport, const u32 x, const u32 y) noexcept;

    private:
        RenderConfig      mConfig;
        integrator::Path  mIntegrator;
        std::vector<Vec3> mFrameBuffer;
};