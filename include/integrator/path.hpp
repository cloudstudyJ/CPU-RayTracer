#pragma once

#include "types.hpp"
#include "math/vec3.hpp"

// fwd
struct Ray;

// fwd
class World;

namespace integrator {
    class Path {
        Path()                           = delete;
        Path(const Path&)                = delete;
        Path(Path&&) noexcept            = delete;
        Path& operator=(const Path&)     = delete;
        Path& operator=(Path&&) noexcept = delete;

        public:
            explicit Path(const u32 maxRayDepth) noexcept;
            ~Path() noexcept = default;

            Vec3 Li(const World& world, const Ray& ray) const;

        private:
            Vec3 hitPass() const;
            Vec3 missPass() const;

        private:
            u32 mMaxRayDepth{};
    };
}