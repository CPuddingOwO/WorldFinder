#include <WorldFinder/game/ecs/world.hpp>
#include <WorldFinder/game/ecs/system.hpp>
#include <WorldFinder/game/ecs/component.hpp>

namespace game::ecs {
    // 碰撞检测函数：检查两个 AABB 是否相交
    bool isCollision(const glm::vec3& pos1, const glm::vec2& size1, const glm::vec3& pos2, const glm::vec2& size2) {
        return (pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x) &&
               (pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y);
    }

    void resolveCollision(glm::vec3& pos1, glm::vec2& vel1, const glm::vec2& size1, const glm::vec3& pos2, const glm::vec2& size2) {
        // 简单处理：发生碰撞时反向速度，避免穿透
        if (pos1.x < pos2.x + size2.x && pos1.x + size1.x > pos2.x) {
            vel1.x = -vel1.x /2;  // X轴反向
        }
        if (pos1.y < pos2.y + size2.y && pos1.y + size1.y > pos2.y) {
            vel1.y = -vel1.y /2;  // Y轴反向
        }
    }

    void World::RegistryComponent() {
        using namespace game::ecs::component;

        world->component<Position>();
        world->component<Velocity>();
        world->component<Collision>();
    }

    void World::RegistrySystem() {
        using namespace game::ecs::system;
        world->system<Position, Collision, Velocity>("Render")
                .each([&](flecs::entity e, Position& pos, Collision& col, Velocity& vel) {
                      graphics->addRect({pos.x, pos.y}, {col.width, col.height}, {255, 0, 0, 255}, true);
                      graphics->addRect({pos.x, pos.y}, {4, 4}, {255, 255, 0, 255}, true);
//                    graphics->addText(std::format("ID: {}", e.name().c_str()), {pos.x-(col.width/2), pos.y-col.height-10});
//                    graphics->addText(std::format("V: ({:.4f}, {:.4f})", vel.x, vel.y), {pos.x, pos.y});
//                    graphics->addText(std::format("P: ({:.4f}, {:.4f})", pos.x, pos.y), {pos.x, pos.y+10});

                    // Bro 第一次做碰撞检测，挺好的，就是硬冲还是能穿过去
                    world->query<Position, Velocity, Collision>().each([&](flecs::entity e2, Position& pos2, Velocity& vel2, Collision& col2) {

                        if (e.id() == e2.id()) return ;

                        if (isCollision({pos.x, pos.y, pos.z}, {col.width, col.height}, {pos2.x, pos2.y, pos2.z}, {col2.width, col2.height})) {
                            if (pos.x < pos2.x + col.width && pos.x + pos.x > pos2.x) {
                                vel.x = -vel.x / 2;  // X轴反向
                            }
                            if (pos.y < pos2.y + col2.height && pos.y + col.height > pos2.y) {
                                vel.y = -vel.y / 2;  // Y轴反向
                            }
                        }
                    });
                });
        world->system<Velocity>().each(VelocitySystem);
        world->system<Position, Velocity>().each(MovementSystem);
    }

    flecs::entity World::AddEntity(const std::string& name) {
        return world->entity(name.c_str());
    }
}
