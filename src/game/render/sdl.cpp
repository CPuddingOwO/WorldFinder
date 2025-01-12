#include <glad/glad.h>
#include <WorldFinder/game/render/sdl.hpp>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace game::render::sdl {
    Graphics::Graphics(const std::shared_ptr<di::DependencyInjector>& injector, const GraphicsOptions& options) {
        this->options_ = options;
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            throw std::runtime_error("Error: SDL_Init(): " + std::string(SDL_GetError()));
        }

        this->window_ = createSDLWindow(options);
        this->renderer_ = nullptr; // Unused; Using OpenGL
        // this->renderer = createSDLRenderer(this->window, options);
        this->glContext_ = createSDLGLContext(this->window_);
        SDL_GL_MakeCurrent(this->window_, this->glContext_);
        setVSync(this->options_.isVsync);
//        setBorderless(this->options_.isBorderless);
        setBorderless(false);

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            throw std::runtime_error("Failed to initialize OpenGL!");
        }
        glViewport(0, 0, this->options_.size.x, this->options_.size.y);

        std::string vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 pos;
uniform mat4 projection;
uniform mat4 model;
void main() {
    gl_Position = projection * model * vec4(pos, 1.0);
}
)";

        std::string fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 color;
void main() {
    FragColor = color;
}
)";
        this->shaderProgram_ = createShaderProgram(vertexShaderSource, fragmentShaderSource);
        if (this->shaderProgram_ == 0) {
            spdlog::error("Shader program creation failed!");
        } else {
            spdlog::debug("Shader program created successfully!");
        }

        glGenBuffers(1, &this->vertexBufferID_); // 生成一个缓冲区对象
    }

    Graphics::~Graphics() {
        destroySDLWindow(*this->window_);
        // destroySDLRenderer(*this->renderer); // Unused; Using OpenGL
        destroySDLGLContext(this->glContext_);
        SDL_Quit();
    }

    SDL_Window* Graphics::createSDLWindow(const GraphicsOptions& op) {
        SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true);
        SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING,
                              op.title.c_str());
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER,
                              op.size.x);
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER,
                              op.size.y);
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN,
                              op.isBorderless); // Borderless
        SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN, true); // HDPI

        // Init Window
        SDL_Window* wind = SDL_CreateWindowWithProperties(props);
        if (wind == nullptr) {
            throw std::runtime_error("Error: SDL_CreateWindow(): " + std::string(SDL_GetError()));
        }
        SDL_DestroyProperties(props);
//        SDL_Surface *surface = IMG_Load("res/images/logo.png");
//        SDL_SetWindowIcon(wind, surface);
//        SDL_DestroySurface(surface);
        return wind;
    }

    SDL_Renderer* Graphics::createSDLRenderer(SDL_Window* wind, const GraphicsOptions& op) {
         SDL_PropertiesID props = SDL_CreateProperties();
         SDL_SetStringProperty(props, SDL_PROP_RENDERER_CREATE_NAME_STRING, "OpenGL");
         SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, wind);
         SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER,
                               op.isVsync ? 1 : 0);
         SDL_Renderer* rend = SDL_CreateRendererWithProperties(props);
         if (rend == nullptr) {
             throw std::runtime_error("Error: SDL_CreateRenderer(): " + std::string(SDL_GetError()));
         }
         SDL_DestroyProperties(props);
        return rend;
    }

    SDL_GLContext Graphics::createSDLGLContext(SDL_Window *wind) {
        auto ctx = SDL_GL_CreateContext(wind);
        if (ctx == nullptr) {
            throw std::runtime_error("Error: SDL_GL_CreateContext(): " + std::string(SDL_GetError()));
        }
        return ctx;
    }

    Graphics &Graphics::setVSync(bool isVsync) {
        this->options_.isVsync = isVsync;
        SDL_GL_SetSwapInterval( isVsync ? 1 : 0);
        return *this;
    }

    Graphics &Graphics::setBorderless(bool isBorderless) {
        this->options_.isBorderless = isBorderless;
        SDL_SetWindowBordered(this->window_, !isBorderless);
        return *this;
    }

    Graphics& Graphics::setScale(float x, float y) {
        this->options_.scale.x = x;
        this->options_.scale.y = y;

        return *this;
    }

    Graphics &Graphics::addRect(const glm::ivec2 &pos, const glm::ivec2 &size, const glm::ivec4& color, bool isCenterBottom) {

//        SDL_FRect rect = {(float)pos.x, (float)pos.y, (float)size.x, (float)size.y};
//        if (isCenterBottom) {
//            rect.x -= (float)size.x / 2;
//            rect.y -= (float)size.y;
//        }
//        SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
//        SDL_RenderRect(this->renderer, &rect);
//        return *this;

        // 计算矩形的顶点位置
        auto x = (float)pos.x;
        auto y = (float)pos.y;
        auto width = (float)size.x;
        auto height = (float)size.y;

        if (isCenterBottom) {
            rect.x -= (float)size.x / 2;
            rect.y -= (float)size.y;
        }
        SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
        SDL_RenderRect(this->renderer, &rect);
        return *this;
    }

    Graphics &Graphics::addPoint(const glm::ivec2 &pos) {
        SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
        SDL_RenderPoint(this->renderer, (float)pos.x, (float)pos.y);
        return *this;
    }

    void Graphics::onRender() {
//        SDL_RenderPresent(this->renderer);
//        SDL_SetRenderDrawColor(this->renderer, 88, 129, 87, 255 );
//        SDL_RenderClear(this->renderer);
        SDL_GL_SwapWindow(this->window_);

        glClearColor(88.0f / 255.0f, 129.0f / 255.0f, 87.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    bool Graphics::checkShaderCompilation(GLuint shader) {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint logLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            std::string infoLog(logLength, ' ');
            glGetShaderInfoLog(shader, logLength, &logLength, &infoLog[0]);
            spdlog::error("Shader compilation failed: {}", infoLog); // TODO: Throw exception
            return false;
        }
        spdlog::trace("Compilation successful for shader {}", shader);
        return true;
    }

    bool Graphics::checkProgramLinking(GLuint program) {
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            GLint logLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
            std::string infoLog(logLength, ' ');
            glGetProgramInfoLog(program, logLength, &logLength, &infoLog[0]);
            spdlog::error("Program linking failed: {}", infoLog); // TODO: Throw exception
            return false;
        }
        spdlog::trace("Linking successful for shader program {}", program);
        return true;
    }

    GLuint Graphics::createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
        // Compile vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexSource = vertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &vertexSource, nullptr);
        glCompileShader(vertexShader);
        if (!checkShaderCompilation(vertexShader)) {
            glDeleteShader(vertexShader);
            return 0;
        }

        // Compile fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentSource = fragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
        glCompileShader(fragmentShader);
        if (!checkShaderCompilation(fragmentShader)) {
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return 0;
        }

        // Create program and link shaders
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        if (!checkProgramLinking(shaderProgram)) {
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteProgram(shaderProgram);
            return 0;
        }

        // Clean up individual shaders as they are no longer needed
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Return the created and linked shader program
        return shaderProgram;
    }

}
