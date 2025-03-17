#pragma once

#include "component.hpp"

namespace wf::game::ecs::system {
    using namespace game::ecs::component;

    void InputSystem(flecs::entity e, ImpulseAcceleration& i_acc);
    void AccelerateSystem(const flecs::entity& e, Velocity& vel, ContinuousAcceleration& c_acc, ImpulseAcceleration& i_acc);
    void DragSystem(Velocity& vel, const Drag& drag);
    void MovementSystem(Position& pos, const Velocity& vel);
    void GravitySystem(const Gravity& gra, const Mass& mass, ContinuousAcceleration& c_acc, const Position& pos);

    void RenderSystem(flecs::entity e,const Position& pos, const Sprite& sprite);
}