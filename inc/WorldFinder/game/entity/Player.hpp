#pragma once

#include <WorldFinder/game/entity/interface/IEntity.hpp>
#include <WorldFinder/game/entity/interface/IMove.hpp>
#include <WorldFinder/game/entity/interface/IPhysics.hpp>

#include <WorldFinder/game/ecs/world.hpp>

#include <string>

using namespace wf::game::entity::interface;
using namespace wf::game::ecs::component;

namespace wf::game::entity {
    class Player : public Entity, public IMove, public IPhysics {
    private:
        std::string name;
    public:
        void init_component() override;
        void init_physics_component() override;
        void init_move_component() override;

        Player(std::string name, flecs::world* world);
        void jump() override;
        void go_left() override;
        void go_right() override;

        bool is_touch_floor() override;
        bool is_touch_wall() override;
        bool is_touch_ceiling() override;

        bool will_touch_floor() override;
    };
}