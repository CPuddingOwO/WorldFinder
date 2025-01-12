#include <WorldFinder/App.hpp>
#include <spdlog/spdlog.h>
#include <WorldFinder/game/render/sdl.hpp>
#include <WorldFinder/game/render/fpsmanager.hpp>

using namespace game;

namespace WorldFinder {
    App::App(const std::shared_ptr<di::DependencyInjector>& injector, const AppOptions& options) {
        this->injector = injector;
        this->options = options;

        this->init();
    }

    App::~App() { this->quit(); }

    bool App::init() {
        spdlog::info("Initializing app");
        spdlog::trace("w: {}, h: {}", options.width, options.height );
        { // Construct graphics
//            auto inj = std::make_shared<di::DependencyInjector>();
            auto op = render::sdl::GraphicsOptions();
            op.title = this->options.title;
            op.size = {this->options.width, this->options.height};
            op.isBorderless = false;
            op.isVsync = true;
            this->gfx = std::make_shared<render::sdl::Graphics>(nullptr, op);
            gfx->setScale(3, 3);
        }
        { // Construct ecs world
            auto inj = std::make_shared<di::DependencyInjector>();
            inj->RegisterDependency<render::sdl::Graphics>(this->gfx);
            auto op = game::ecs::WorldOptions();
            ecs = std::make_shared<game::ecs::World>(inj, op);
        }

        ecs->AddEntity("player")
            .set<game::ecs::component::Position>({64, 16, 0})
            .set<game::ecs::component::Velocity>({2, 2})
            .set<game::ecs::component::Collision>({32, 32});
        ecs->AddEntity("test1")
            .set<game::ecs::component::Position>({32, 16, 0})
            .set<game::ecs::component::Velocity>({0, 0})
            .set<game::ecs::component::Collision>({32, 32});
        return false;
    }

    bool App::run() {
        spdlog::info("Running app");
        while(options.isRunning) {
            injector->GetDependency<render::FpsManager>()->start();
            {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_EVENT_QUIT)
                        this->options.isRunning = false;
                    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(this->gfx->getWindow()))
                        this->options.isRunning = false;

                    auto player = ecs->GetWorld().entity("player");
                    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_D) {
                        auto vel = player.get<game::ecs::component::Velocity>();
                        player.set<game::ecs::component::Velocity>({vel->x + 0.75, vel->y});
                    }
                    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_A) {
                        auto vel = player.get<game::ecs::component::Velocity>();
                        player.set<game::ecs::component::Velocity>({vel->x - 0.75, vel->y});
                    }
                    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_W) {
                        auto vel = player.get<game::ecs::component::Velocity>();
                        player.set<game::ecs::component::Velocity>({vel->x, vel->y - 0.75});
                    }
                    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_S) {
                        auto vel = player.get<game::ecs::component::Velocity>();
                        player.set<game::ecs::component::Velocity>({vel->x, vel->y + 0.75});
                    }
                }

            }

            for (size_t i = 0; i < 32; i++) {
                gfx->addRect({0, 8*i}, {256, 1}, {58, 90, 64, 255}, false); // 红色水平线段，起始位置 (10, 20)，长度 200
                gfx->addRect({8*i, 0}, {1, 128+64}, {58, 90, 64, 255}, false); // 绿色垂直线段，起始位置 (20, 10)，长度 150
            }

//            gfx->addText(std::format("FPS: {}", injector->GetDependency<render::FpsManager>()->getDelta()), {10, 10});
            ecs->GetWorld().progress(15.0f);

            gfx->onRender();
            injector->GetDependency<render::FpsManager>()->stop();
        }
        return false;
    }

    bool App::quit() {
        options.isRunning = false; // None
        spdlog::info("Quitting app");
        return false;
    }

}