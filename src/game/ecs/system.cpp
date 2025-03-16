#include <flecs.h>
#include <WorldFinder/game/ecs/system.hpp>
#include <spdlog/spdlog.h>

#include "WorldFinder/Input.hpp"

namespace wf::game::ecs::system {
    void AccelerateSystem(Velocity& vel, const Acceleration& acc) {
        // if (vel.x != 0) vel.x += acc.x;
        // if (vel.y != 0) vel.y += acc.y;
    }

    void DragSystem(Velocity& vel, const Drag& drag) {
        double threshold = 0.05;

        if (std::abs(vel.x) < threshold ) vel.x = 0; else vel.x *= drag.value;
        if (std::abs(vel.y) < threshold ) vel.y = 0; else vel.y *= drag.value;
    }

    void MovementSystem(Position& pos, Velocity& vel) {
        if (vel.x != 0) pos.x += vel.x;
        if (vel.y != 0) pos.y += vel.y;
        // if (vel.x != 0 && vel.y != 0) spdlog::trace("Position: ({}, {}, {})", pos.x, pos.y, pos.z);
    }

    void RenderSystem(flecs::entity e,const Position& pos, const Sprite& sprite) {
        // auto* graphics = static_cast<render::sdl::Graphics*>(e.world().get_ctx());
        const auto* injector = static_cast<di::DependencyInjector*>(e.world().get_ctx());
        const auto graphics = injector->GetDependency<render::sdl::Graphics>();
        graphics->addRect({pos.x, pos.y}, {sprite.size.x, sprite.size.y}, {52, 78, 65, 255}, true);
        graphics->addRect({pos.x, pos.y}, {4, 4}, {255, 255, 0, 255}, true);
        // graphics->addText(std::format("ID: {}", e.name().c_str()), {pos.x-(sprite.size.x/2), pos.y-col.y-10});
        // graphics->addText(std::format("V: ({:.4f}, {:.4f})", vel.x, vel.y), {pos.x, pos.y});
        // graphics->addText(std::format("P: ({:.4f}, {:.4f})", pos.x, pos.y), {pos.x, pos.y+10});
    }

    void InputSystem(flecs::entity e, Velocity& vel) {
        const auto* injector = static_cast<di::DependencyInjector*>(e.world().get_ctx());
        const auto input = injector->GetDependency<input::KeyboardStats>();

        if (input->is_pressed(input::key::W)) vel.y -= 0.2f;
        if (input->is_pressed(input::key::S)) vel.y += 0.2f;
        if (input->is_pressed(input::key::A)) vel.x -= 0.2f;
        if (input->is_pressed(input::key::D)) vel.x += 0.2f;
    }

}
