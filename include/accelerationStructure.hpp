#pragma once

#include "aabb.hpp"
#include "hitResult.hpp"

#include <vector>

// fwd
struct Ray;
class Object;

class AccelerationStructure {
    private:
        struct Node {
            AABB aabb;

            Node* left{};
            Node* right{};

            Object* obj{};
        };

    public:
        ~AccelerationStructure() noexcept;

        void build(const std::vector<Object*>& objects);

        HitResult hitTest(const Ray& ray, const f32 tMin, const f32 tMax) noexcept;

    private:
        Node* build(std::vector<Object*>& objRefs, const u32 from, const u32 to);

        Node* traversal(const Ray& ray, const f32 tMin, const f32 tMax);

    private:
        Node* mRoot{};
};