#include <WorldFinder/App.hpp>
#include <WorldFinder/game/ecs/world.hpp>

#include <memory>
#include <spdlog/spdlog.h>
#include <WorldFinder/game/render/fpsmanager.hpp>
#include <SDL3/SDL_main.h>


using namespace wf;

int main([[maybe_unused]]int argc, [[maybe_unused]]char* argv[]) {
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("%^[%H:%M:%S][%L] %v%$");
    auto options = AppOptions();
    options.setTitle("World Finder")
           .setWidth(720*2)
           .setHeight(480*2);

    auto injector = std::make_shared<di::DependencyInjector>();

    auto fpsmanager = std::make_shared<game::render::FpsManager>(-1);
    injector->RegisterDependency<game::render::FpsManager>(fpsmanager);

    try {
        const auto app = std::make_unique<App>(injector, options);
        app->run();

    }catch(const std::exception& e) {
        spdlog::error(e.what());
    }

    return 0;
}
