#pragma once

#include <SDL3/SDL_video.h>
#include <WorldFinder/game/render/adapter.hpp>

namespace wf::game::render {
    struct OpenGLRendererAdapterOption {
        std::string title;
        bool isVsync;
        bool isBorderless;
        glm::vec<2, int> size;
    };

    class OpenGLRendererAdapter final : public Adapter {
    private:
        OpenGLRendererAdapterOption option;
        SDL_Window* window;
        SDL_GLContext glContext;

        GLuint shaderProgram{};  // 着色器程序
        GLuint vertexArrayID{};  // 顶点数组对象
        GLuint vertexBufferID{}; // 顶点缓冲区对象
        GLuint fontTexture{}; // 顶点缓冲区对象

        ~OpenGLRendererAdapter() override;

        static SDL_Window* CreateSDLWindow(const OpenGLRendererAdapterOption& op);
        static void DestroySDLWindow(SDL_Window& wind) { SDL_DestroyWindow(&wind); }

        static SDL_GLContext CreateSDLGLContext(SDL_Window* wind);
        static void DestroySDLGLContext(const SDL_GLContext& context) { SDL_GL_DestroyContext(context); }

        /// 待重构
        bool CheckShaderCompilation(GLuint shader);
        bool CheckProgramLinking(GLuint program);
        GLuint BuildShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        GLuint LoadFontTexture(const std::string& fontPath);
    public:
        explicit OpenGLRendererAdapter(const OpenGLRendererAdapterOption& op);
        Result Initialize() override;
        Result Submit(const Drawlist& drawlist) override;
        Result Reload() override;
        Result Destroy() override;
        /// --- /// 一下为私有函数，每个渲染器都不一样
    private:

    };
}