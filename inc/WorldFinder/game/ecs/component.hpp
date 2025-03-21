#pragma once
#include <WorldFinder/game/render/graphics.hpp>

namespace wf::game::ecs::component {

    struct Position {
        double x, y;
    };

    struct Velocity {
        glm::vec<2, double> cur, max;
    };

    struct Acceleration {
        double x, y;
    };

    // 持续加速度组件
    struct ContinuousAcceleration : Acceleration {
        double x, y;
    };

    // 瞬时加速度组件
    struct ImpulseAcceleration : Acceleration {
        double x, y;
    };

    struct Drag {
        double friction;
    };

    struct Sprite {
        glm::ivec2 size;
    };

    struct Gravity {
        double g;
    };

    struct Mass {
        double m;
    };
}
