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

        world->component<Position>();
        world->component<Velocity>();
        world->component<Acceleration>();
        world->component<Drag>();
    }

    void World::RegistrySystem() {
        using namespace game::ecs::system;
        // world->system<Position>("Position")
        //         .each([&](flecs::entity e, Position& pos) {
        //
        //         });
        world->system<Velocity>().kind(flecs::OnUpdate).each(InputSystem);
        world->system<Velocity, const Acceleration>("AccelerationSystem").kind(flecs::OnUpdate).each(AccelerateSystem);
        world->system<Velocity, const Drag>("DragSystem").kind(flecs::OnUpdate).each(DragSystem);
        world->system<Position, Velocity>("MovementSystem").kind(flecs::OnUpdate).each(MovementSystem);
        world->system<const Position, const Sprite>("RenderSystem").kind(flecs::OnUpdate).each(RenderSystem);
    }

    flecs::entity World::AddEntity(const std::string& name) {
        return world->entity(name.c_str());
    }
}
