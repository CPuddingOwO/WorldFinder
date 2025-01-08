#pragma once

#include <flecs.h>
#include <memory>
#include <unordered_map>
#include "component.hpp"
#include "system.hpp"

namespace game::ecs {
    class World {
    public:
        World() {
            world = std::make_unique<flecs::world>();
            RegistryComponent();
            RegistrySystem();
        }

        flecs::world& GetWorld() { return *world; }

        ~World() = default;

    private:
        void RegistryComponent();
        void RegistrySystem();

        std::unique_ptr<flecs::world> world;
    };

}  // namespace ecs
