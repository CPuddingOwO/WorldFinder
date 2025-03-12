#pragma once

namespace wf::game::entity::interface {
    class IMove {
    public:
        virtual void init_move_component() = 0;
        virtual void go_left() = 0;
        virtual void go_right() = 0;
        virtual void jump() = 0;
    };
}