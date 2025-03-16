#pragma once

#include <WorldFinder/game/entity/interface/Entity.hpp>
#include <WorldFinder/game/ecs/world.hpp>

using namespace wf::game::entity::interface;
// using namespace wf::game::ecs::component;

namespace wf::game::entity {
    class Player : public Entity{
    private:
    public:
        explicit Player(flecs::entity e) {
            this->entity = e;
        }
    };
}