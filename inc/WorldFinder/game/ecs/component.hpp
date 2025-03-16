#pragma once
#include <WorldFinder/game/render/sdl.hpp>

namespace wf::game::ecs::component {

    struct Position {
        double x, y;
    };

    struct Velocity {
        double x, y;
    };

    struct Acceleration {
        double x, y;
    };

    struct Drag {
        double value;
    };

    struct Sprite {
        glm::ivec2 size;
    };
}
