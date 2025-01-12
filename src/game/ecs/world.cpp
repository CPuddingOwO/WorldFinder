#include <WorldFinder/game/ecs/world.hpp>
#include <WorldFinder/game/ecs/system.hpp>
#include <WorldFinder/game/ecs/component.hpp>
#include <spdlog/spdlog.h>

namespace game::ecs {
    void World::RegistryComponent() {
        using namespace game::ecs::component;

        world->component<Position>();
        world->component<Velocity>();
        world->component<Collision>();
    }

    void World::RegistrySystem() {
        using namespace game::ecs::system;
        world->system<Velocity>().each(VelocitySystem);
        world->system<Position, Collision, Velocity>("Render")
                .each([&](flecs::entity e, Position& pos, Collision& col, Velocity& vel) {
                      graphics->addRect({pos.x, pos.y}, {col.x, col.y}, {52, 78, 65, 255}, true);
                      graphics->addRect({pos.x, pos.y}, {4, 4}, {255, 255, 0, 255}, true);
//                    graphics->addText(std::format("ID: {}", e.name().c_str()), {pos.x-(col.width/2), pos.y-col.height-10});
//                    graphics->addText(std::format("V: ({:.4f}, {:.4f})", vel.x, vel.y), {pos.x, pos.y});
//                    graphics->addText(std::format("P: ({:.4f}, {:.4f})", pos.x, pos.y), {pos.x, pos.y+10});

                    // Bro 第一次做碰撞检测，挺好的，就是硬冲还是能穿过去
                    world->query<Position, Velocity, Collision>().each([&](flecs::entity e2, Position& pos2, Velocity& vel2, Collision& col2) {

                        if (e.id() == e2.id()) return ;

                        if ((pos.x < pos2.x + col2.x && pos.x + col.x > pos2.x) && (pos.y < pos2.y + col.y && pos.y + col.y > pos2.y)) {

                            spdlog::trace("<Collied!> {} => {} at ({:.4f}, {:.4f})", e.name().c_str(), e2.name().c_str(), pos.x, pos.y);

                            if (pos.x < pos2.x + col2.x && pos.x + col.x > pos2.x) {
//                                vel.x = -vel.x / 2;
                                pos.x += -vel.x;
                                vel.x = 0;
                            }
                            if (pos.y < pos2.y + col2.y && pos.y + col.y > pos2.y) {
//                                vel.y = -vel.y / 2;
                                pos.y += -vel.y;
                                vel.y = 0;
                            }
                        }
                    });
                });
        world->system<Position, Velocity>().each(MovementSystem);
    }

    flecs::entity World::AddEntity(const std::string& name) {
        return world->entity(name.c_str());
    }
}
