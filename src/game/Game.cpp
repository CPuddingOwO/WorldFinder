#include <WorldFinder/game/Game.hpp>
#include <WorldFinder/game/entity/Player.hpp>

namespace wf::game {
    Game::Game(const std::shared_ptr<di::DependencyInjector> &injector, const GameOptions &options): state() {
        this->injector = injector;
        this->options = options;
        {
            // 构造 EcsWorld 实例
            // 并注入 Graphics gfx 到 EcsWorld
            auto inj = std::make_shared<di::DependencyInjector>();
            inj->RegisterDependency<Graphics>(injector->GetDependency<Graphics>());
            auto op = WorldOptions();
            this->ecs = std::make_shared<World>(inj, op);
        }
    }

    Result Game::Initialize() {
        ecs->GetWorld().import<flecs::stats>();
        ecs->GetWorld().set<flecs::Rest>({27750});

        auto player =std::make_shared<entity::Player>("Player", &ecs->GetWorld());
        player->init_component();
        injector->RegisterDependency<entity::Player>(player);
        return Result();
    }

    Result Game::HandleEvent() {
        return Result();
    }

    Result Game::Update() {
        ecs->Update();
        return Result();
    }


    Result Game::Render() {
        return Result();
    }


    Result Game::Destroy() {
        return Result();
    }


}
