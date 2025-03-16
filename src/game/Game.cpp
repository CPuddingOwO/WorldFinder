#include <WorldFinder/game/Game.hpp>
#include <WorldFinder/game/entity/Player.hpp>

#include "WorldFinder/game/ecs/component.hpp"

namespace wf::game {
    Game::Game(const std::shared_ptr<di::DependencyInjector> &injector, const GameOptions &options): state() {
        this->injector = injector;
        this->options = options;
        this->keyboard_stats = injector->GetDependency<input::KeyboardStats>();
        {
            // 构造 EcsWorld 实例
            // 并注入 Graphics gfx 到 EcsWorld
            auto inj = std::make_shared<di::DependencyInjector>();
            inj->RegisterDependency<Graphics>(injector->GetDependency<Graphics>());
            inj->RegisterDependency<input::KeyboardStats>(keyboard_stats);
            auto op = WorldOptions();
            this->ecs = std::make_shared<World>(inj, op);
        }
        this->gfx = injector->GetDependency<Graphics>();
    }

    Result Game::Initialize() {
        // 设置Flecs的WebExplorer`
        ecs->GetWorld().import<flecs::stats>();
        ecs->GetWorld().set<flecs::Rest>({27750});

        {
            // 初始化玩家
            auto player = ecs->AddEntity("Player")
                 .set<component::Sprite>({{16, 16}})
                 .set<component::Position>({0, 32})
                 .set<component::Velocity>({3, 0})
                 .set<component::Drag>({0.90});
            auto p = std::make_shared<entity::Player>(player);
            this->injector->RegisterDependency<entity::Player>(p);

        }
        return Result();
    }

    Result Game::HandleEvent() {
        // 不知道这个后续还要不要
        return Result();
    }

    Result Game::Update() { // 暂不考虑 Const
        ecs->Update(); // 更新ecs
        return Result();
    }


    Result Game::Render() {
        // 将缓冲区的数据渲染到屏幕
        gfx->onRender();
        return Result();
    }


    Result Game::Destroy() {
        // 销毁自身 (当前Game对象)
        return Result();
    }


}
