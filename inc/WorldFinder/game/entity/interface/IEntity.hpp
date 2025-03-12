#pragma once
#include <flecs.h>

namespace wf::game::entity {
    class Entity {
    public:
        virtual ~Entity() = default;
        virtual void init_component() = 0;
    protected:
        flecs::entity entity;
    };
}