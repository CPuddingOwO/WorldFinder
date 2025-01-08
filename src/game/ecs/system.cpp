#include <WorldFinder/game/ecs/system.hpp>
#include <spdlog/spdlog.h>

namespace game::ecs::system {
    void MovementSystem(Position& pos, Velocity& vel) {
        if (vel.x != 0) pos.x += vel.x;
        if (vel.y != 0) pos.y += vel.y;
        if (vel.x != 0 && vel.y != 0) spdlog::trace("Position: ({}, {}, {})", pos.x, pos.y, pos.z);
    }

    void VelocitySystem(Velocity& vel) {
        double friction = 0.80;
        if (vel.x < 0.0001) vel.x =0; else vel.x *= friction;
        if (vel.y < 0.0001) vel.y = 0; else vel.y *= friction;
        if (vel.x != 0 && vel.y != 0) spdlog::trace("Velocity: ({}, {})", vel.x, vel.y);
    }
}