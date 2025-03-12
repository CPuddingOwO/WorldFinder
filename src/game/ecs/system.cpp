#include <flecs/addons/cpp/entity.hpp>
#include <flecs/addons/cpp/iter.hpp>
#include <WorldFinder/game/ecs/system.hpp>
#include <spdlog/spdlog.h>

namespace wf::game::ecs::system {
    void PhysicsSystem(Transform& trs) {
        double friction = 0.90f;
        //        double friction = 0.80f;
        //        double friction = 0.95f;
        double threshold = 0.00001;
        if (std::abs(trs.vel.x) < threshold ) trs.vel.x = 0; else trs.vel.x *= friction;
        if (std::abs(trs.vel.y) < threshold ) trs.vel.y = 0; else trs.vel.y *= friction;
        //        if (vel.x != 0 || vel.y != 0) spdlog::trace("Velocity: ({}, {})", vel.x, vel.y);
        if (trs.vel.x != 0) trs.pos.x += trs.vel.x;
        if (trs.vel.y != 0) trs.pos.y += trs.vel.y;
//        if (vel.x != 0 && vel.y != 0) spdlog::trace("Position: ({}, {}, {})", pos.x, pos.y, pos.z);
    }

    void RenderSystem(flecs::entity e, flecs::iter& it, Transform& trs, Collision& col) {
        auto* graphics = static_cast<render::sdl::Graphics*>(it.world().get_context());
        graphics->addRect({trs.pos.x, trs.pos.y}, {col.x, col.y}, {52, 78, 65, 255}, true);
        graphics->addRect({trs.pos.x, trs.pos.y}, {4, 4}, {255, 255, 0, 255}, true);
        graphics->addText(std::format("ID: {}", e.name().c_str()), {trs.pos.x-(col.x/2), trs.pos.y-col.y-10});
        graphics->addText(std::format("V: ({:.4f}, {:.4f})", trs.vel.x, trs.vel.y), {trs.pos.x, trs.pos.y});
        graphics->addText(std::format("P: ({:.4f}, {:.4f})", trs.pos.x, trs.pos.y), {trs.pos.x, trs.pos.y+10});
    }

    void InputSystem() {

    }

}