#pragma once
#include <flecs.h>

namespace wf::game::entity::interface {
    class Entity {
    public:
        virtual ~Entity() = default;
    protected:
        flecs::entity entity;
    };
}