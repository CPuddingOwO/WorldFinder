#include <WorldFinder/game/ecs/world.hpp>
#include <WorldFinder/game/ecs/system.hpp>
#include <WorldFinder/game/ecs/component.hpp>
#include <spdlog/spdlog.h>

namespace wf::game::ecs {
    void World::RegistryQuery() {
        using namespace game::ecs::component;
    }

    void World::RegistryComponent() const {
        using namespace game::ecs::component;

        world->component<Transform>();
        world->component<Collision>();
        world->component<Physical>();
    }

    void World::RegistrySystem() {
        using namespace game::ecs::system;
        // world->system<Position, Collision, Velocity>("Render")
        //         .each([&](flecs::entity e, Position& pos, Collision& col, Velocity& vel) {
        //
        //         });
        world->system<Transform, Collision>("RenderSystem").kind(flecs::OnUpdate).each(RenderSystem);
        world->system<Transform>("PhysicsSystem").each(PhysicsSystem);
    }

    flecs::entity World::AddEntity(const std::string& name) {
        return world->entity(name.c_str());
    }
}
