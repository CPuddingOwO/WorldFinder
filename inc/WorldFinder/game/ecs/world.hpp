#pragma once

#include <flecs.h>
#include <memory>
#include <unordered_map>
#include <string>
#include "component.hpp"
#include "system.hpp"
#include <WorldFinder/di/DependencyInjector.hpp>
#include <WorldFinder/game/render/sdl.hpp>

namespace wf::game::ecs {
    using namespace game::render::sdl;
    struct WorldOptions {

    };

    class World {
    private:
        WorldOptions options;
        std::shared_ptr<di::DependencyInjector> injector;
        std::shared_ptr<Graphics> graphics;
    public:
        World(std::shared_ptr<di::DependencyInjector>& injector, const WorldOptions& options) {
            this->options = options;
            this->injector = injector;
            graphics = injector->GetDependency<Graphics>();
            this->world = std::make_unique<flecs::world>();
            this->world->set_ctx(graphics.get());
            RegistryComponent();
            RegistryQuery();
            RegistrySystem();
        }

        bool Update(float dt = 0) {
            return  GetWorld().progress(dt);
        }

        flecs::world& GetWorld() { return *world; }
        ~World() = default;

        flecs::entity AddEntity(const std::string& name);

    private:
        void RegistryComponent() const;
        void RegistrySystem();

        static void RegistryQuery();

        std::unique_ptr<flecs::world> world;
    };

}  // namespace ecs
