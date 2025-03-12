#pragma once

#include "component.hpp"

namespace wf::game::ecs::system {
    using namespace game::ecs::component;

    void RenderSystem(Transform& trs, Collision& col);
    void PhysicsSystem(Transform& trs);
    void InputSystem();
}