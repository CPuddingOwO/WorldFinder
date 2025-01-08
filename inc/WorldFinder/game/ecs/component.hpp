#pragma once

namespace game::ecs::component {
    struct Position {
        double x; // < - >
        double y; // ^ - v
        double z; // X - ·
    };

    struct Velocity {
        double x; // < - >
        double y; // ^ - v
        // No Z
    };
}