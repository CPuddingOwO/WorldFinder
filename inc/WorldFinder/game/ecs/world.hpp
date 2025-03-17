#pragma once

#include <flecs.h>
#include <memory>
#include <string>
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
            this->graphics = injector->GetDependency<Graphics>();
            this->world = std::make_unique<flecs::world>();

            this->world->set_ctx(this->injector.get()); // 直接注入依赖

            RegistryComponent();
            RegistryQuery();
            RegistrySystem();
        }

        bool Update(float dt = 20) {
            return  GetWorld().progress();
        }

        [[nodiscard]] flecs::world& GetWorld() const { return *world; }
        ~World() = default;

        flecs::entity AddEntity(const std::string& name) const;

    private:
        void RegistryComponent() const;
        void RegistrySystem() const;
        void RegistryQuery() const;
        // void RegisterObserver();

        std::unique_ptr<flecs::world> world;
    };

}  // namespace ecs
