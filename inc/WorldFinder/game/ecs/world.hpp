#pragma once

#include <flecs.h>
#include <memory>
#include <unordered_map>
#include <string>
#include "component.hpp"
#include "system.hpp"
#include <WorldFinder/di/DependencyInjector.hpp>
#include <WorldFinder/game/render/sdl.hpp>

namespace game::ecs {
    using namespace game::render::sdl;
    struct WorldOptions {

    };

    class World {
    private:
        WorldOptions options;
        std::shared_ptr<Graphics> graphics;
    public:
        World(std::shared_ptr<di::DependencyInjector>& injector, const WorldOptions& options) {
            this->options = options;
            graphics = injector->GetDependency<Graphics>();
            world = std::make_unique<flecs::world>();
            RegistryComponent();
            RegistrySystem();
        }

        flecs::world& GetWorld() { return *world; }
        ~World() = default;

        flecs::entity AddEntity(const std::string& name);

    private:
        void RegistryComponent();
        void RegistrySystem();

        std::unique_ptr<flecs::world> world;
    };

}  // namespace ecs
