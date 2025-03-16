#pragma once

#include "component.hpp"

namespace wf::game::ecs::system {
    using namespace game::ecs::component;

    void InputSystem(flecs::entity e, Velocity& vel);
    void AccelerateSystem(Velocity& vel, const Acceleration& acc);
    void DragSystem(Velocity& vel, const Drag& drag);
    void MovementSystem(Position& pos, Velocity& vel);

    void RenderSystem(flecs::entity e,const Position& pos, const Sprite& sprite);
}