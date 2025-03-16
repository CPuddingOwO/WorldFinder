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
        {   // 初始化 KeyboardStats 实例
            keyboard_stats = std::make_shared<input::KeyboardStats>();
        }
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
            // 注入 Graphics: gfx 到 Game 实例中
            // 注入 KeyboardStats: keyboard_stats 到 Game
            auto inj = std::make_shared<di::DependencyInjector>();
            inj->RegisterDependency<sdl::Graphics>(this->gfx);
            inj->RegisterDependency<input::KeyboardStats>(keyboard_stats);
            auto op = game::GameOptions();
            game = std::make_shared<game::Game>(inj, op);
        }

        this->game->Initialize();
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

                const auto state = SDL_GetKeyboardState(nullptr);
                memcpy(keyboard_stats->keys.data(), state, SDL_SCANCODE_COUNT);
                // for (size_t i = 0; i < SDL_SCANCODE_COUNT; i++) {
                    // keyboard_stats->keys.at(i) = state[i];
                // }
                // if (state[SDL_SCANCODE_D])      { player->go_right(); }
                // if (state[SDL_SCANCODE_A])      { player->go_left(); }
                // if (state[SDL_SCANCODE_SPACE])  { player->jump(); }

            }
            this->game->Update();
            this->game->Render();
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