#include <glad/glad.h>
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <WorldFinder/game/render/adapter/OpenGLRendererAdapter.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace wf::game::render {
    OpenGLRendererAdapter::OpenGLRendererAdapter(const OpenGLRendererAdapterOption& op): window(nullptr), glContext(nullptr) {
        this->option = op;
        SDL_GL_MakeCurrent(this->window, this->glContext);
        // setVSync(this->options.isVsync);
        //        setBorderless(this->options_.isBorderless);
        // setBorderless(false);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
            throw std::runtime_error("Failed to initialize OpenGL!");
        }
        glViewport(0, 0, this->option.size.x, this->option.size.y);

        const std::string vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 pos;
uniform mat4 projection;
uniform mat4 model;
void main() {
    gl_Position = projection * model * vec4(pos, 1.0);
}
)";

        const std::string fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 color;
void main() {
    FragColor = color;
}
)";
        this->shaderProgram = BuildShaderProgram(vertexShaderSource, fragmentShaderSource);
        if (this->shaderProgram == 0) {
            spdlog::error("Shader program creation failed!");
        } else {
            spdlog::debug("Shader program created successfully!");
        }

        glGenBuffers(1, &this->vertexBufferID); // 生成一个缓冲区对象

        // this->fontTexture = LoadFontTexture("./font.png");
    }

    OpenGLRendererAdapter::~OpenGLRendererAdapter() {
        DestroySDLWindow(*this->window);
        DestroySDLGLContext(this->glContext);
    }

    SDL_Window * OpenGLRendererAdapter::CreateSDLWindow(const OpenGLRendererAdapterOption &op) {
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
        return wind;
    }

    SDL_GLContext OpenGLRendererAdapter::CreateSDLGLContext(SDL_Window *wind) {
        const auto ctx = SDL_GL_CreateContext(wind);
        if (ctx == nullptr) {
            throw std::runtime_error("Error: SDL_GL_CreateContext(): " + std::string(SDL_GetError()));
        }
        return ctx;
    }

    Result OpenGLRendererAdapter::Initialize() {
        return Result();
    }

    Result OpenGLRendererAdapter::Submit(const Drawlist& drawlist) {
        SDL_GL_SwapWindow(this->window);

        glClearColor(88.0f / 255.0f, 129.0f / 255.0f, 87.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        return Result();
    }

    Result OpenGLRendererAdapter::Reload() {
        return Result();
    }

    Result OpenGLRendererAdapter::Destroy() {
        return Result();
    }

    bool OpenGLRendererAdapter::CheckShaderCompilation(GLuint shader) {
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

    bool OpenGLRendererAdapter::CheckProgramLinking(GLuint program) {
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

    GLuint OpenGLRendererAdapter::BuildShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
        // Compile vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexSource = vertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &vertexSource, nullptr);
        glCompileShader(vertexShader);
        if (!CheckShaderCompilation(vertexShader)) {
            glDeleteShader(vertexShader);
            return 0;
        }

        // Compile fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentSource = fragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
        glCompileShader(fragmentShader);
        if (!CheckShaderCompilation(fragmentShader)) {
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return 0;
        }

        // Create program and link shaders
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        if (!CheckProgramLinking(shaderProgram)) {
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

    GLuint OpenGLRendererAdapter::LoadFontTexture(const std::string& fontPath) {
        // TODO: Load font texture
        int width, height, channels;
        unsigned char* data = stbi_load(fontPath.c_str(), &width, &height, &channels, 0);
        if (!data) {
            spdlog::error("Failed to load font texture: {}", fontPath);
            return 0;
        }

        GLuint fontTexture;
        glGenTextures(1, &fontTexture);
        glBindTexture(GL_TEXTURE_2D, fontTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);


        stbi_image_free(data);

        return fontTexture;
    }
}
