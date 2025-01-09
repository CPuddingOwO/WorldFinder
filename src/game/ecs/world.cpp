#include <WorldFinder/game/ecs/world.hpp>
#include <WorldFinder/game/ecs/system.hpp>
#include <WorldFinder/game/ecs/component.hpp>

namespace game::ecs {
    void World::RegistryComponent() {
        using namespace game::ecs::component;

        world->component<Position>();
        world->component<Velocity>();
        world->component<Collision>();
    }

    void World::RegistrySystem() {
        using namespace game::ecs::system;

        world->system<Position, Velocity>().each(MovementSystem);
        world->system<Velocity>().each(VelocitySystem);
        world->system<Position, Collision>("Render")
                .each([&](flecs::entity e, Position& pos, Collision& col) {
                    graphics->addRect({pos.x, pos.y}, {col.width, col.height}, true);
                });
    }

    flecs::entity World::AddEntity(const std::string& name) {
        return world->entity(name.c_str());
    }
}
