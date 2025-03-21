#pragma once

#include <memory>
#include <WorldFinder/di/DependencyInjector.hpp>
#include <WorldFinder/game/ecs/world.hpp>
#include <WorldFinder/game/render/graphics.hpp>
#include "game/Game.hpp"
#include <WorldFinder/Input.hpp>



namespace wf {
    struct AppOptions {
        std::string title;
        bool isRunning = true;
        int width = 0, height = 0;

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
    };

    class App {
        AppOptions options;
        std::shared_ptr<input::KeyboardStats> keyboard_stats;
        std::shared_ptr<di::DependencyInjector> injector{};

        std::shared_ptr<game::render::gfx::Graphics> gfx;
        std::shared_ptr<game::Game> game;
    public:
        App(const std::shared_ptr<di::DependencyInjector>& injector, const AppOptions& options);
        ~App();

        bool init();
        bool run();
        bool quit();
    };
}
