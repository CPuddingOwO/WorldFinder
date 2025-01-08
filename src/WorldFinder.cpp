#include <WorldFinder/App.hpp>
#include <WorldFinder/game/ecs/world.hpp>

#include <memory>
#include <spdlog/spdlog.h>


using namespace WorldFinder;

int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("%^[%H:%M:%S][%L] %v%$");

    auto options = AppOptions();
    options.setWidth(16).setHeight(16).setIsRunning(true);

    auto injector = std::make_shared<di::DependencyInjector>();

    auto ecs = std::make_shared<game::ecs::World>();
    injector->RegisterDependency<game::ecs::World>(ecs);

    auto app = std::make_unique<App>(injector, options);
    app->run();
    return 0;
}
