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

        world->system<Velocity>().each(VelocitySystem);
        world->system<Position, Velocity>().each(MovementSystem);
        world->system<Position, Collision, Velocity>("Render")
                .each([&](flecs::entity e, Position& pos, Collision& col, Velocity& vel) {
                      graphics->addRect({pos.x, pos.y}, {col.width, col.height}, {255, 0, 0, 255}, true);
                      graphics->addRect({pos.x, pos.y}, {4, 4}, {255, 255, 0, 255}, true);
//                    graphics->addText(std::format("ID: {}", e.name().c_str()), {pos.x-(col.width/2), pos.y-col.height-10});
//                    graphics->addText(std::format("V: ({:.4f}, {:.4f})", vel.x, vel.y), {pos.x, pos.y});
//                    graphics->addText(std::format("P: ({:.4f}, {:.4f})", pos.x, pos.y), {pos.x, pos.y+10});
                });
    }

    flecs::entity World::AddEntity(const std::string& name) {
        return world->entity(name.c_str());
    }
}
