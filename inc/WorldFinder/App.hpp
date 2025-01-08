#pragma once

#include <memory>
#include <WorldFinder/di/DependencyInjector.hpp>
#include <WorldFinder/game/ecs/world.hpp>

namespace WorldFinder {
    struct AppOptions {
        int width = 0, height = 0;
        bool isRunning = false;

        AppOptions& setWidth(int w) {
            this->width = w;
            return *this;
        }

        AppOptions& setHeight(int h) {
            this->height = h;
            return *this;
        }

        AppOptions& setIsRunning(bool flag) {
            this->isRunning = flag;
            return *this;
        }
    };

    class App {
    private:
        AppOptions options;
        std::shared_ptr<game::ecs::World> ecs;
    public:
        App(const std::shared_ptr<di::DependencyInjector>& injector, AppOptions options);
        ~App();

        bool init();
        bool run();
        bool quit();
    };
}