#include <WorldFinder/game/ecs/world.hpp>
#include <WorldFinder/game/ecs/system.hpp>
#include <WorldFinder/game/ecs/component.hpp>

namespace game::ecs {
    void World::RegistryComponent() {
        using namespace game::ecs::component;

        world->component<Position>();
        world->component<Velocity>();
    }

    void World::RegistrySystem() {
        using namespace game::ecs::system;

        world->system<Position, Velocity>().each(MovementSystem);
        world->system<Velocity>().each(VelocitySystem);
    }
}
