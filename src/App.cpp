#include <WorldFinder/App.hpp>
#include <spdlog/spdlog.h>
#include "WorldFinder/game/render/sdl.hpp"

using namespace game;

namespace WorldFinder {
    App::App(const std::shared_ptr<di::DependencyInjector>& injector, const AppOptions& options) {
//        this->injector = injector;
        this->options = options;

        this->init();
    }

    App::~App() { this->quit(); }

    bool App::init() {
        spdlog::info("Initializing app");
        spdlog::trace("w: {}, h: {}", options.width, options.height );
        { // Construct graphics
            auto inj = std::make_shared<di::DependencyInjector>();
            auto op = render::sdl::GraphicsOptions();
            op.title = this->options.title;
            op.size = {this->options.width, this->options.height};
            op.isBorderless = false;
            this->gfx = std::make_shared<render::sdl::Graphics>(inj, op);
        }
        { // Construct ecs world
            auto inj = std::make_shared<di::DependencyInjector>();
            inj->RegisterDependency<render::sdl::Graphics>(this->gfx);
            auto op = game::ecs::WorldOptions();
            ecs = std::make_shared<game::ecs::World>(inj, op);
        }

        ecs->AddEntity("player")
            .set<game::ecs::component::Position>({0, 0, 0})
            .set<game::ecs::component::Velocity>({5, 5})
            .set<game::ecs::component::Collision>({5, 5});
        return false;
    }

    bool App::run() {
        spdlog::info("Running app");
        while(options.isRunning) {
            {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_EVENT_QUIT)
                        this->options.isRunning = false;
                    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(this->gfx->getWindow()))
                        this->options.isRunning = false;
                }

            }

            gfx->onRender();

            ecs->GetWorld().progress();
        }

        return false;
    }

    bool App::quit() {
        options.isRunning = false; // None
        spdlog::info("Quitting app");
        return false;
    }

}