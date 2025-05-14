#include <flecs.h>
#include <WorldFinder/game/ecs/system.hpp>
#include <spdlog/spdlog.h>

#include "WorldFinder/Input.hpp"
#include "WorldFinder/game/scene/TileManager.hpp"

namespace wf::game::ecs::system {
    void AccelerateSystem(const flecs::entity& e, Velocity& vel, const ImpulseAcceleration& i_acc) {
        const auto* c_acc = e.get<ContinuousAcceleration>();
        Acceleration acc{};
        if (c_acc) {
            acc.x = c_acc->x + i_acc.x;
            acc.y = c_acc->y + i_acc.y;
            e.remove<ContinuousAcceleration>();
        } else {
            acc.x = i_acc.x;
            acc.y = i_acc.y;
        }
        if (glm::abs(vel.cur.x) < vel.max.x) {
            if (glm::abs(vel.cur.x + acc.x) >= vel.max.x) {
                vel.cur.x = (vel.cur.x + acc.x > 0) ? vel.max.x : -vel.max.x;
            } else {
                vel.cur.x += acc.x;
            }
        }
        if (glm::abs(vel.cur.y) < vel.max.y) {
            if (glm::abs(vel.cur.y + acc.y) >= vel.max.y) {
                vel.cur.y = (vel.cur.y + acc.y > 0) ? vel.max.y : -vel.max.y;
            } else {
                vel.cur.y += acc.y;
            }
        }
        e.set<ImpulseAcceleration>({0, 0});
    }

    void DragSystem(Velocity& vel, const Drag& drag) {
        constexpr double threshold = 0.02;

        if (std::abs(vel.cur.x) < threshold ) vel.cur.x = 0; else vel.cur.x *= drag.friction;
        if (std::abs(vel.cur.y) < threshold ) vel.cur.y = 0; else vel.cur.y *= drag.friction;
    }

void CollisionSystem(const flecs::entity &e, Position &pos, Velocity &vel) {
    const auto *injector = static_cast<di::DependencyInjector *>(e.world().get_ctx());
    const auto tm = injector->GetDependency<game::scene::TileManager>();

    // 假设实体有一个大小 (entitySize)
    const glm::vec2 entitySize = {32, 64};

    // 保存原始位置，用于计算位移
    const glm::vec2 originalPos = {pos.x, pos.y};

    // 由于实体中心点在脚底中央，调整边界计算
    // 查询可能碰撞的瓦片 (扩大查询范围以确保捕获所有可能的碰撞)
    const auto tiles = tm->query({pos.x - entitySize.x / 2 - 5, pos.y - entitySize.y - 5},
                                {entitySize.x + 10, entitySize.y + 10});

    for (const auto &tile: tiles) {
        // 计算实体和瓦片的边界 (调整为实体中心点在脚底中央)
        const double entityLeft = pos.x - entitySize.x / 2;
        const double entityRight = pos.x + entitySize.x / 2;
        const double entityTop = pos.y - entitySize.y; // 顶部边界从脚底向上entitySize.y
        const double entityBottom = pos.y;             // 底部边界就是pos.y位置

        const float tileLeft = tile.pos.x;
        const float tileRight = tile.pos.x + tile.size.x;
        const float tileTop = tile.pos.y;
        const float tileBottom = tile.pos.y + tile.size.y;

        // 检查是否发生碰撞
        const bool collisionX = entityRight > tileLeft && entityLeft < tileRight;
        const bool collisionY = entityBottom > tileTop && entityTop < tileBottom;

        if (collisionX && collisionY) {
            // 计算碰撞的深度（在x和y方向）
            const double depthX = std::min(entityRight - tileLeft, tileRight - entityLeft);
            const double depthY = std::min(entityBottom - tileTop, tileBottom - entityTop);

            // 确定碰撞的主要方向并仅在该方向应用反弹
            if (depthX < depthY) {
                // 水平方向的碰撞 - 确定是左边还是右边
                if (originalPos.x < tile.pos.x + tile.size.x / 2) {
                    // 从左边碰撞
                    pos.x = tileLeft - entitySize.x / 2 - 0.01f; // 轻微偏移防止粘在墙上
                    if (vel.cur.x > 0) vel.cur.x = -vel.cur.x * 0.3f; // 向左反弹，减小速度
                } else {
                    // 从右边碰撞
                    pos.x = tileRight + entitySize.x / 2 + 0.01f;
                    if (vel.cur.x < 0) vel.cur.x = -vel.cur.x * 0.3f; // 向右反弹，减小速度
                }
            } else {
                // 垂直方向的碰撞 - 确定是上边还是下边
                if (originalPos.y - entitySize.y/2 < tile.pos.y + tile.size.y / 2) {
                    // 从上方碰撞
                    // pos.y = tileTop - entitySize.y + 0.01f; // 注意这里的调整
                    if (vel.cur.y > 0) vel.cur.y = -vel.cur.y * 0.3f; // 向上反弹，减小速度
                } else {
                    // 从下方碰撞
                    // pos.y = tileBottom + 0.01f; // 注意这里的调整
                    if (vel.cur.y < 0) vel.cur.y = -vel.cur.y * 0.3f; // 向下反弹，减小速度
                }
            }

            // 移除了全局弹射效果，改为在各个方向分别处理
        }
    }
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
        const auto tm = injector->GetDependency<game::scene::TileManager>();
        const auto tiles = tm->query({0, 0}, {720*2, 480*2}); // TODO: qwq
        for (const auto& tile : tiles) {
            graphics->addRect({tile.pos.x, tile.pos.y}, {tile.size.x, tile.size.y}, {0, 0, 0, 255}, true, false);
        }
        graphics->submit();
        graphics->addRect({pos.x, pos.y}, {sprite.size.x, sprite.size.y}, {52, 78, 65, 255}, true, true);
        graphics->addRect({pos.x, pos.y}, {4, 4}, {255, 255, 0, 255}, true, true);
        // graphics->addText(std::format("ID: {}", e.name().c_str()), {pos.x-(sprite.size.x/2), pos.y-sprite.size.y-10});
        // graphics->addText(std::format("V: ({:.4f}, {:.4f})", vel.cur.x, vel.cur.y), {pos.x, pos.y});
        // graphics->addText(std::format("P: ({:.4f}, {:.4f})", pos.x, pos.y), {pos.x, pos.y+10}, {255, 255, 255, 255}, true);
        graphics->submit();
    }

    void InputSystem(const flecs::entity e, ImpulseAcceleration& i_acc) {
        const auto* injector = static_cast<di::DependencyInjector*>(e.world().get_ctx());
        const auto input = injector->GetDependency<input::KeyboardStats>();
        Acceleration acc{};

        if (input->is_pressed(input::key::SPACE)) acc.y += -0.5f; else acc.y = 0;
        if (input->is_pressed(input::key::A)) acc.x += -0.2f; else acc.x = 0;
        if (input->is_pressed(input::key::S)) acc.y += 0.2f;  else acc.y == 0 ? acc.y = 0 : acc.y;
        if (input->is_pressed(input::key::D)) acc.x += 0.2f;  else acc.x == 0 ? acc.x = 0 : acc.x;
        i_acc.x += acc.x;
        i_acc.y += acc.y;
    }

    void GravitySystem(const flecs::entity e, const Gravity& gra, const Mass& mass, Position& pos, Velocity& vel) {
        // 创建一个常量表示地面高度
        const auto injector = static_cast<di::DependencyInjector*>(e.world().get_ctx());
        const auto tm = injector->GetDependency<game::scene::TileManager>();
        const auto tiles = tm->query({pos.x, pos.y}, {1, 16});
        float GROUND_LEVEL = 480*2;
        if (!tiles.empty()) {
            GROUND_LEVEL = tiles[0].pos.y;
        }


        // 检查是否已经在地面上
        bool isOnGround = (pos.y >= GROUND_LEVEL);

        // 创建连续加速度组件
        ContinuousAcceleration c_acc{};

        if (!isOnGround) {
            // 如果不在地面上，应用重力
            c_acc.y = mass.m * gra.g;
        } else {
            // 如果在地面上，停止下落并设置正确的y位置
            c_acc.y = 0;
            pos.y = GROUND_LEVEL;  // 确保物体正好停在地面上
            vel.cur.y = 0;         // 重置垂直速度为0
        }

        // 更新实体的连续加速度
        e.set<ContinuousAcceleration>(c_acc);

        // 可选：添加IsGrounded组件标记物体是否在地面上，方便其他系统使用
        if (isOnGround) {
            if (!e.has<IsGrounded>()) {
                e.add<IsGrounded>();
            }
        } else {
            if (e.has<IsGrounded>()) {
                e.remove<IsGrounded>();
            }
        }
    }


}
