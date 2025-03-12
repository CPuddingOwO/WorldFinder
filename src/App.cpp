#include <WorldFinder/App.hpp>
#include <spdlog/spdlog.h>
#include <WorldFinder/game/render/sdl.hpp>
#include <WorldFinder/game/render/fpsmanager.hpp>

#include <WorldFinder/game/entity/Player.hpp>

#include "WorldFinder/game/Game.hpp"


using namespace wf::game::render;

namespace wf {
    App::App(const std::shared_ptr<di::DependencyInjector>& injector, const AppOptions& options) {
        this->injector = injector;
        this->options = options;
        this->init();
    }

    App::~App() { this->quit(); }

    bool App::init() {
        spdlog::info("Initializing app");
        spdlog::trace("w: {}, h: {}", options.width, options.height );
        {   // 构造 Graphics: gfx 图形类
            auto op = sdl::GraphicsOptions();
            op.title = this->options.title;
            op.size = {this->options.width, this->options.height};
            op.isBorderless = false;
            op.isVsync = true;
            this->gfx = std::make_shared<sdl::Graphics>(nullptr, op);
            gfx->setScale(3, 3);
        }
        {   // 构造 Game 实例
            // 并 注入 Graphics: gfx 到 Game 实例中
            auto inj = std::make_shared<di::DependencyInjector>();
            inj->RegisterDependency<sdl::Graphics>(this->gfx);
            auto op = game::GameOptions();
            game = std::make_shared<game::Game>(inj, op);
        }
        return false;
    }

    bool App::run() {
        spdlog::info("Running app");
        while(options.isRunning) {
            injector->GetDependency<FpsManager>()->start();
            {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_EVENT_QUIT)
                        this->options.isRunning = false;
                    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(this->gfx->getWindow()))
                        this->options.isRunning = false;
                }

                auto player = injector->GetDependency<game::entity::Player>();
                auto state = SDL_GetKeyboardState(nullptr);
                if (state[SDL_SCANCODE_D])      { player->go_right(); }
                if (state[SDL_SCANCODE_A])      { player->go_left(); }
                if (state[SDL_SCANCODE_SPACE])  { player->jump(); }

            }
            auto player = injector->GetDependency<game::entity::Player>();

//            for (size_t i = 0; i < 32; i++) {
//                gfx->addRect({0, 8*i}, {256, 1}, {58, 90, 64, 255}, false); // 红色水平线段，起始位置 (10, 20)，长度 200
//                gfx->addRect({8*i, 0}, {1, 128+64}, {58, 90, 64, 255}, false); // 绿色垂直线段，起始位置 (20, 10)，长度 150
//            }
            // gfx->addRect({0, 128}, {720, 1}, {58, 90, 64, 255}, false); // 红色水平线段，起始位置 (10, 20)，长度 200

//            gfx->addText(std::format("FPS: {}", injector->GetDependency<render::FpsManager>()->getDelta()), {10, 10});

            this->game->Update();

            gfx->onRender();
            injector->GetDependency<FpsManager>()->stop();
        }
        return false;
    }

    bool App::quit() {
        options.isRunning = false; // None
        spdlog::info("Quitting app");
        return false;
    }

}