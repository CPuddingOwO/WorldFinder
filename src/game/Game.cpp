#include <WorldFinder/game/Game.hpp>
#include <WorldFinder/game/entity/Player.hpp>

#include "WorldFinder/game/ecs/component.hpp"
#include "WorldFinder/game/scene/DefaultScene.hpp"

namespace wf::game {
    Game::Game(const std::shared_ptr<di::DependencyInjector> &injector, const GameOptions &options): state() {
        this->injector = injector;
        this->options = options;
        this->keyboard_stats = injector->GetDependency<input::KeyboardStats>();
        this->tm = std::make_shared<TileManager>(glm::vec<2, float>{0, 0}, glm::vec<2, float>{this->options.screen_size.x, this->options.screen_size.y});
        {
            // 构造 EcsWorld 实例
            // 并注入 Graphics gfx 到 EcsWorld
            auto inj = std::make_shared<di::DependencyInjector>();
            inj->RegisterDependency<Graphics>(injector->GetDependency<Graphics>());
            inj->RegisterDependency<input::KeyboardStats>(keyboard_stats);
            inj->RegisterDependency<TileManager>(this->tm);
            auto op = WorldOptions();
            this->ecs = std::make_shared<World>(inj, op);
        }
        this->gfx = injector->GetDependency<Graphics>();
    }

    Result Game::Initialize() {
        // 设置Flecs的WebExplorer`
        // ecs->GetWorld().import<flecs::stats>();
        // ecs->GetWorld().set<flecs::Rest>({27750});
        {
            // 初始场景 Default Scene
            auto inj = std::make_shared<di::DependencyInjector>();
            inj->RegisterDependency<TileManager>(this->tm);
            inj->RegisterDependency<World>(this->ecs);
            this->options.scene = std::make_shared<DefaultScene>(inj);
            this->options.scene->Initialize();
        }


        return Result();
    }

    Result Game::HandleEvent() {
        // 不知道这个后续还要不要
        return Result();
    }

    Result Game::Update() { // 暂不考虑 Const
        ecs->Update(20.0/1000); // 更新ecs
        return Result();
    }


    Result Game::Render() {

        glm::vec<4, int> col = {110, 139, 116, 255};
        {
            // 绘制网格
            short blod_grid_spacing = 0;
            constexpr int reg_grid_spacing = 32;
            const glm::vec<2, int> grid_size = {options.screen_size.x/reg_grid_spacing, options.screen_size.y/reg_grid_spacing};
            for (int i = 0; i < grid_size.x; i+= 1) { // 绘制横向网格
                if (blod_grid_spacing == 5) {
                    gfx->addRect({0+i*reg_grid_spacing-1, 0}, {3, options.screen_size.y}, col, true, false);
                    blod_grid_spacing = 0;
                } else {
                    gfx->addRect({0+i*reg_grid_spacing, 0}, {1, options.screen_size.y}, col, true, false);
                }
                blod_grid_spacing +=1;
            }
            blod_grid_spacing = 0;
            for (int i = 0; i < grid_size.y; i+= 1) {
                if (blod_grid_spacing == 5) { // 绘制纵向网格
                    gfx->addRect({0, 0+i*reg_grid_spacing-1}, {options.screen_size.x, 3}, col, true, false);
                    blod_grid_spacing = 0;
                } else {
                    gfx->addRect({0, 0+i*reg_grid_spacing}, {options.screen_size.x, 1}, col, true, false);
                }
                blod_grid_spacing +=1;
            }
            // gfx->addRect({0, 256-1}, {options.screen_size.x, 3}, {117, 49, 23, 255}, true);
            gfx->submit();
        }


        gfx->render();
        return Result();
    }


    Result Game::Destroy() {
        this->options.scene->Destroy();
        return Result();
    }


}
