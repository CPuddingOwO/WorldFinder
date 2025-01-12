#include <WorldFinder/App.hpp>
#include <WorldFinder/game/ecs/world.hpp>

#include <memory>
#include <spdlog/spdlog.h>
#include <WorldFinder/game/render/fpsmanager.hpp>


using namespace WorldFinder;

int main([[maybe_unused]]int argc, [[maybe_unused]]char* argv[]) {
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("%^[%H:%M:%S][%L] %v%$");

    auto options = AppOptions();
    options.setTitle("World Finder")
           .setWidth(720)
           .setHeight(480)
           .setIsRunning(true);

    auto injector = std::make_shared<di::DependencyInjector>();

    auto fpsmanager = std::make_shared<game::render::FpsManager>(60.0f);
    injector->RegisterDependency<game::render::FpsManager>(fpsmanager);

    auto app = std::make_unique<App>(injector, options);
    app->run();
    return 0;
}
