#pragma once

#include "component.hpp"

namespace game::ecs::system {
    using namespace game::ecs::component;

    void MovementSystem(Position& pos, Velocity& vel);
    void VelocitySystem(Velocity& vel);
}