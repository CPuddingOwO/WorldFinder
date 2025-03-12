#pragma once
#include <glm/vec2.hpp>
#include <WorldFinder/game/render/sdl.hpp>

namespace wf::game::ecs::component {
    // struct Position {
    //     double x; // < - >
    //     double y; // ^ - v
    //     double z; // X - ·
    // };
    //
    // struct Velocity {
    //     double x; // < - >
    //     double y; // ^ - v
    //     // No Z
    // };
    struct GraphicsContext {
        render::sdl::Graphics* instance;
    };

    struct Collision {
        double x;
        double y;
    };

    struct Transform {
        glm::vec2<double> vel;
        glm::vec2<double> pos;
    };

    struct Physical {
        bool is_gravity_active  = true;
        double gravity_value    = 0.0;

        bool is_touch_floor     = false;
        bool is_touch_ceiling   = false;
        bool is_touch_wall      = false;
        bool will_touch_floor   = false;
        bool will_touch_ceiling = false;
        bool will_touch_wall    = false;
    };
}
