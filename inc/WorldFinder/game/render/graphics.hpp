#pragma once
#include <SDL3/SDL.h>
#include <WorldFinder/di/DependencyInjector.hpp>
#include <glm/glm.hpp>
#include <string>
#include "adapter.hpp"
#include "drawlist.hpp"
#include "SDL3/SDL_opengl.h"

namespace wf::game::render::gfx {
    struct GraphicsOptions {
        std::string title = "Application";
        glm::ivec2 size = {680, 480};
        glm::fvec2 scale = {1.0f, 1.0f};
        bool isBorderless = false;
        bool isVsync = true;
    };

    class Graphics {
        GraphicsOptions options;
        std::shared_ptr<Drawlist> drawlist;
        std::shared_ptr<Adapter> adapter;
    public:
        Graphics(const std::shared_ptr<di::DependencyInjector>& injector , const GraphicsOptions& options);
        ~Graphics();
        Graphics& setScale(float x, float y);
        Graphics& setVSync(bool isVsync);
        Graphics& setBorderless(bool isBorderless);

        Graphics& render() {
            this->adapter->Submit(*this->drawlist);
            this->drawlist->Clear();
            return *this;
        }
        Graphics& submit() {this->drawlist->SubmitDrawCommand(nullptr); return *this;}

        Graphics& addRect(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec4& color, bool fill = false, bool center_bottom = false);
        // Graphics& addPoint(const glm::ivec2& pos);
        // Graphics& addText(const std::string& text, const glm::ivec2& pos, const glm::ivec4 &color, bool isCenterBottom);
    private:
    };
}