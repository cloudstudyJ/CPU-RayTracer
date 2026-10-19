#pragma once

#include "aabb.hpp"
#include "hitResult.hpp"

#include <vector>
#include <memory>

// fwd
struct Ray;
class Object;

class AccelerationStructure {
    AccelerationStructure(const AccelerationStructure&) = delete;
    AccelerationStructure& operator=(const AccelerationStructure&) = delete;

    private:
        struct Node {
            AABB aabb;

            Node* left {};
            Node* right{};

            Object* obj{};
        };

    public:
        AccelerationStructure() noexcept = default;
        AccelerationStructure(AccelerationStructure&& other) noexcept;
        ~AccelerationStructure() noexcept;

        AccelerationStructure& operator=(AccelerationStructure&& other) noexcept;

        void build(const std::vector<std::unique_ptr<Object>>& objects);
        void clear() noexcept;

        HitResult hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const noexcept;

    private:
        Node* build(std::vector<Object*>& objRefs, const u32 from, const u32 to);

    private:
        Node* mRoot{};
};