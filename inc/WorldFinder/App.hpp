#pragma once

#include <memory>
#include <WorldFinder/di/DependencyInjector.hpp>
#include <WorldFinder/game/ecs/world.hpp>
#include <WorldFinder/game/render/sdl.hpp>
#include <utility>

namespace WorldFinder {
    struct AppOptions {
        std::string title;
        int width = 0, height = 0;
        bool isRunning = false;

        AppOptions& setTitle(std::string t) {
            this->title = std::move(t);
            return *this;
        }

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
        std::shared_ptr<di::DependencyInjector> injector{};

        std::shared_ptr<game::ecs::World> ecs;
        std::shared_ptr<game::render::sdl::Graphics> gfx;
    public:
        App(const std::shared_ptr<di::DependencyInjector>& injector, const AppOptions& options);
        ~App();

        bool init();
        bool run();
        bool quit();
    };
}