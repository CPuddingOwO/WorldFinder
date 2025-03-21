#include <flecs.h>
#include <WorldFinder/game/ecs/system.hpp>
#include <spdlog/spdlog.h>

#include "WorldFinder/Input.hpp"

namespace wf::game::ecs::system {
    void AccelerateSystem(const flecs::entity& e, Velocity& vel, ContinuousAcceleration& c_acc, ImpulseAcceleration& i_acc) {
        // const Acceleration acc = {c_acc.x + i_acc.x, c_acc.y + i_acc.y};
        const Acceleration acc = {c_acc.x + i_acc.x, c_acc.y + i_acc.y};
        if (vel.cur.x < vel.max.x) {
            if (vel.cur.x + acc.x > vel.max.x) vel.cur.x = vel.max.x;
            else vel.cur.x += acc.x;
        }
        if (vel.cur.y < vel.max.y) {
            if (vel.cur.y + acc.y > vel.max.y) vel.cur.y = vel.max.y;
            else vel.cur.y += acc.y;
        }
        i_acc = {0, 0};
    }

    void DragSystem(Velocity& vel, const Drag& drag) {
        constexpr double threshold = 0.02;

        if (std::abs(vel.cur.x) < threshold ) vel.cur.x = 0; else vel.cur.x *= drag.friction;
        if (std::abs(vel.cur.y) < threshold ) vel.cur.y = 0; else vel.cur.y *= drag.friction;
    }

    void MovementSystem(Position& pos, const Velocity& vel) {
        if (vel.cur.x != 0) pos.x += vel.cur.x;
        if (vel.cur.y != 0) pos.y += vel.cur.y;
        // if (vel.cur.x != 0 && vel.cur.y != 0) spdlog::trace("Position: ({}, {}, {})", pos.x, pos.y, pos.z);
    }

    void RenderSystem(flecs::entity e,const Position& pos, const Sprite& sprite) {
        // auto* graphics = static_cast<render::sdl::Graphics*>(e.world().get_ctx());
        const auto* injector = static_cast<di::DependencyInjector*>(e.world().get_ctx());
        const auto graphics = injector->GetDependency<render::gfx::Graphics>();
        graphics->addRect({pos.x, pos.y}, {sprite.size.x, sprite.size.y}, {52, 78, 65, 255}, true, true);
        graphics->addRect({pos.x, pos.y}, {4, 4}, {255, 255, 0, 255}, true, true);
        // graphics->addText(std::format("ID: {}", e.name().c_str()), {pos.x-(sprite.size.x/2), pos.y-sprite.size.y-10});
        // graphics->addText(std::format("V: ({:.4f}, {:.4f})", vel.cur.x, vel.cur.y), {pos.x, pos.y});
        // graphics->addText(std::format("P: ({:.4f}, {:.4f})", pos.x, pos.y), {pos.x, pos.y+10}, {255, 255, 255, 255}, true);
        graphics->submit();
    }

    void InputSystem(flecs::entity e, ImpulseAcceleration& i_acc) {
        const auto* injector = static_cast<di::DependencyInjector*>(e.world().get_ctx());
        const auto input = injector->GetDependency<input::KeyboardStats>();

        if (input->is_pressed(input::key::W)) i_acc.y = -0.2f;
        if (input->is_pressed(input::key::S)) i_acc.y = 0.2f;
        if (input->is_pressed(input::key::A)) i_acc.x = -0.2f;
        if (input->is_pressed(input::key::D)) i_acc.x = 0.2f;
    }

    void GravitySystem(const Gravity& gra, const Mass& mass, ContinuousAcceleration& c_acc, const Position& pos) {
        if (pos.y < 256) c_acc.y = mass.m * gra.g;
        else c_acc.y = 0;
    }

}
