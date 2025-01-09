#pragma once
#include <SDL3/SDL.h>
#include <WorldFinder/di/DependencyInjector.hpp>
#include <glm/glm.hpp>
#include <string>

namespace game::render::sdl {
    struct GraphicsOptions {
        std::string title = "Application";
        glm::ivec2 size = {680, 480};
        bool isBorderless = true;
        bool isVsync = true;
    };

    class Graphics {
    private:
        GraphicsOptions options;
        SDL_Window* window{};
        SDL_Renderer* renderer{};
    public:
        Graphics(const std::shared_ptr<di::DependencyInjector>& injector, const GraphicsOptions& options);
        ~Graphics();

        [[nodiscard]] SDL_Window* getWindow() const { return window; };
        [[nodiscard]] SDL_Renderer* getRenderer() const { return renderer; };

        void onRender();

        Graphics& addRect(const glm::ivec2& pos, const glm::ivec2& size, bool isCentered = false);
    private:
        static SDL_Window* createSDLWindow(const GraphicsOptions& op);
        static SDL_Renderer* createSDLRenderer(SDL_Window* wind, const GraphicsOptions& op);
        static void destroySDLRenderer(SDL_Renderer& rend) { SDL_DestroyRenderer(&rend); };
        static void destroySDLWindow(SDL_Window& wind) { SDL_DestroyWindow(&wind); };
    };
}