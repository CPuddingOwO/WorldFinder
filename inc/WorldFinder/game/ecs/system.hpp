#pragma once

#include "component.hpp"

namespace wf::game::ecs::system {
    using namespace game::ecs::component;

    void InputSystem(flecs::entity e, ImpulseAcceleration& i_acc);
    void AccelerateSystem(const flecs::entity& e, Velocity& vel, const ImpulseAcceleration& i_acc);
    void DragSystem(Velocity& vel, const Drag& drag);
    void CollisionSystem(const flecs::entity& e, Position& pos, Velocity& vel);
    void MovementSystem(Position& pos, const Velocity& vel);
    void GravitySystem(flecs::entity e, const Gravity& gra, const Mass& mass, Position& pos, Velocity& vel);

    void RenderSystem(flecs::entity e,const Position& pos, const Sprite& sprite);
}