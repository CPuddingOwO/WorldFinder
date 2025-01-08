#include <WorldFinder/App.hpp>
#include <spdlog/spdlog.h>

namespace WorldFinder {
    App::App(const std::shared_ptr<di::DependencyInjector>& injector, const AppOptions options) {
//        this->injector = injector;
        this->options = options;
        ecs = injector->GetDependency<game::ecs::World>();

        this->init();
    }

    App::~App() { this->quit(); }

    bool App::init() {
        spdlog::info("Initializing app");
        spdlog::trace("w: {}, h: {}", options.width, options.height );

        ecs->GetWorld().entity("cat")
        .set<game::ecs::component::Position>({0, 0, 0})
        .set<game::ecs::component::Velocity>({5, 5});
        return false;
    }

    bool App::run() {
        spdlog::info("Running app");
        while(options.isRunning) {
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