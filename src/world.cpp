#include "world.hpp"

World::World(World&& other) noexcept
    : mMainCamera(std::move(other.mMainCamera)), mAS(std::move(other.mAS)),  mObjects(std::move(other.mObjects)), mLights(std::move(other.mLights)) {
    other.mMainCamera = {};
    other.mAS         = {};
    other.mObjects    = std::vector<std::unique_ptr<Object>>();
    other.mLights     = {};
}
World::~World() noexcept { clearObjects(); }

World& World::operator=(World&& other) noexcept {
    if (this == &other)
        return *this;

    clearObjects();

    mMainCamera = std::move(other.mMainCamera);
    mAS         = std::move(other.mAS);
    mObjects    = std::move(other.mObjects);
    mLights     = std::move(other.mLights);

    other.mMainCamera = {};
    other.mAS         = {};
    other.mObjects    = std::vector<std::unique_ptr<Object>>();
    other.mLights     = {};

    return *this;
}

void World::addCamera(const Camera& camera) noexcept { mMainCamera = camera; }
void World::clearObjects() noexcept {
    mAS.clear();
    mObjects.clear();
    mLights.clear();
}