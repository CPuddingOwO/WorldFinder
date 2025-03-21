#pragma once

#include <SDL3/SDL_render.h>

#include "../adapter.hpp"

namespace wf::game::render {
    struct SDLRendererAdapterOption {
        std::string title;
        bool isVsync;
        bool isBorderless;
        glm::vec<2, int> size;

    };
    class SDLRendererAdapter final : public Adapter {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDLRendererAdapterOption option;

        [[nodiscard]] SDL_Renderer* getRenderer() const { return this->renderer; };

        static SDL_Window* CreateSDLWindow(const SDLRendererAdapterOption& op);
        static void DestroySDLWindow(SDL_Window& wind) { SDL_DestroyWindow(&wind); };

        static SDL_Renderer* CreateSDLRenderer(SDL_Window* wind, const SDLRendererAdapterOption& op);
        static void DestroySDLRenderer(SDL_Renderer& rend) { SDL_DestroyRenderer(&rend); };
    public:
        ~SDLRendererAdapter() override;
        explicit SDLRendererAdapter(const SDLRendererAdapterOption& op);
        Result Initialize() override;
        Result Submit(const Drawlist& drawlist) override;
        Result Reload() override;
        Result Destroy() override;
    };
}