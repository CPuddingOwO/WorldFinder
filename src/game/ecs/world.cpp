#include <WorldFinder/game/ecs/world.hpp>
#include <WorldFinder/game/ecs/system.hpp>
#include <WorldFinder/game/ecs/component.hpp>
#include <spdlog/spdlog.h>

namespace wf::game::ecs {
    void World::RegistryQuery() const {
        using namespace game::ecs::component;
        // auto q = world->query<Position, Velocity, Acceleration, Drag>();
    }

    void World::RegistryComponent() const {
        using namespace game::ecs::component;

        world->component<Position>();
        world->component<Velocity>();
        world->component<ContinuousAcceleration>();
        world->component<ImpulseAcceleration>();
        world->component<Drag>();
    }

    void World::RegistrySystem() const {
        using namespace game::ecs::system;
        // world->system<Position>("Position")
        //         .each([&](flecs::entity e, Position& pos) {
        //
        //         });
        world->system<const Gravity, const Mass, ContinuousAcceleration, const Position>("GravitySystem").kind(flecs::OnUpdate).each(GravitySystem);
        world->system<ImpulseAcceleration>("InputSystem").kind(flecs::OnUpdate).each(InputSystem);
        world->system<Velocity, ContinuousAcceleration, ImpulseAcceleration>("AccelerationSystem").kind(flecs::OnUpdate).each(AccelerateSystem);
        world->system<Velocity, const Drag>("DragSystem").kind(flecs::OnUpdate).each(DragSystem);
        world->system<Position, Velocity>("MovementSystem").kind(flecs::OnUpdate).each(MovementSystem);
        world->system<const Position, const Sprite>("RenderSystem").kind(flecs::OnUpdate).each(RenderSystem);
    }

    flecs::entity World::AddEntity(const std::string& name) const {
        return world->entity(name.c_str());
    }
}
