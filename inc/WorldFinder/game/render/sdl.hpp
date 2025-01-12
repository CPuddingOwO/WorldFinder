#pragma once
#include <SDL3/SDL.h>
#include <WorldFinder/di/DependencyInjector.hpp>
#include <glm/glm.hpp>
#include <string>
#include "SDL3/SDL_opengl.h"

namespace game::render::sdl {
    struct GraphicsOptions {
        std::string title = "Application";
        glm::ivec2 size = {680, 480};
        glm::fvec2 scale = {1.0f, 1.0f};
        bool isBorderless = false;
        bool isVsync = true;
    };

    class Graphics {
    private:
        GraphicsOptions options_;
        SDL_Window* window_{};
        SDL_Renderer* renderer_{};
        SDL_GLContext glContext_{};

        GLuint shaderProgram_{};  // 着色器程序
        GLuint vertexArrayID_{};  // 顶点数组对象
        GLuint vertexBufferID_{}; // 顶点缓冲区对象
    public:
        Graphics(const std::shared_ptr<di::DependencyInjector>& injector , const GraphicsOptions& options);
        ~Graphics();

        [[nodiscard]] SDL_Window* getWindow() const { return this->window_; };
        [[nodiscard]] SDL_Renderer* getRenderer() const { return this->renderer_; };
        [[nodiscard]] SDL_GLContext getGLContext() const { return this->glContext_; };

        void onRender();
        Graphics& setScale(float x, float y);

        Graphics& setVSync(bool isVsync);
        Graphics& setBorderless(bool isBorderless);

        Graphics& addRect(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec4& color, bool isCenterBottom = false);
        Graphics& addPoint(const glm::ivec2& pos);
        Graphics& addText(const std::string& text, const glm::ivec2& pos);
    private:
        static SDL_Window* createSDLWindow(const GraphicsOptions& op);
        static void destroySDLWindow(SDL_Window& wind) { SDL_DestroyWindow(&wind); };

        static SDL_Renderer* createSDLRenderer(SDL_Window* wind, const GraphicsOptions& op);
        static void destroySDLRenderer(SDL_Renderer& rend) { SDL_DestroyRenderer(&rend); };

        static SDL_GLContext createSDLGLContext(SDL_Window* wind);
        static void destroySDLGLContext(SDL_GLContext& context) { SDL_GL_DestroyContext(context); };

        bool checkShaderCompilation(GLuint shader);
        bool checkProgramLinking(GLuint program);
        GLuint createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    };
}