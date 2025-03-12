#pragma once

namespace wf::game::entity::interface {
    class IPhysics {
    public:
        virtual void init_physics_component() = 0;
        virtual bool is_touch_floor() = 0;
        virtual bool is_touch_wall() = 0;
        virtual bool is_touch_ceiling() = 0;
        virtual bool will_touch_floor() = 0;
};
}