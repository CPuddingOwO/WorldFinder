#include <fstream>
#include <glad/glad.h>
#include <WorldFinder/game/render/sdl.hpp>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace wf::game::render::sdl {
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

        this->font_texture_ = loadFontTexture("./font.png");
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
            x -= width / 2.0f;
            y -= height;
        }

        // 定义矩形的 4 个顶点
        GLfloat vertices[] = {
                x, y, 0.0f,                  // 左下角
                x + width, y, 0.0f,          // 右下角
                x + width, y + height, 0.0f, // 右上角
                x, y + height, 0.0f          // 左上角
        };

        // 激活着色器程序
        glUseProgram(this->shaderProgram_);

        // 绑定顶点缓冲区
        glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // 启用顶点属性
        GLuint posAttrib = glGetAttribLocation(this->shaderProgram_, "pos");
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);
        glEnableVertexAttribArray(posAttrib);

        // 设置投影矩阵
        float left = 0.0f, right = (float)this->options_.size.x, bottom = (float)this->options_.size.y, top = 0.0f;
        glm::mat4 projection = glm::ortho(left, right, bottom, top);
        GLint projectionLoc = glGetUniformLocation(this->shaderProgram_, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // 计算或获取模型矩阵
        auto model = glm::mat4(1.0f); // 初始为单位矩阵
        // 如果有平移、旋转或缩放，可以依次应用变换
//        model = glm::translate(model, glm::vec3(1.0f, 2.0f, 0.0f)); // 例如平移
//        model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // 旋转 -5 度
        model = glm::scale(model, glm::vec3(this->options_.scale.x, this->options_.scale.y, 1.0f)); // 缩放

        // 传递模型矩阵到着色器
        GLint modelLoc = glGetUniformLocation(shaderProgram_, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // 传递模型矩阵

        // 设置颜色（这里你也可以通过 uniform 变量来设置颜色）
        GLint colorLoc = glGetUniformLocation(this->shaderProgram_, "color");
        glUniform4f(colorLoc, (float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f, (float)color.a / 255.0f); // 纯红色

        // 绘制矩形（使用两个三角形）
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // 使用 GL_TRIANGLE_FAN 绘制矩形

        return *this;
    }

    Graphics &Graphics::addPoint(const glm::ivec2 &pos) {
//        SDL_SetRenderDrawColor(this->renderer_, 0, 255, 0, 255);
//        SDL_RenderPoint(this->renderer_, (float)pos.x, (float)pos.y);
//        return *this;
        // 将屏幕坐标转换为标准化设备坐标
        //
        float x = (2.0f * (float)pos.x) / (float)this->options_.size.x - 1.0f;
        float y = 1.0f - (2.0f * (float)pos.y) / (float)this->options_.size.y;

        // 创建一个包含点位置的顶点数组
        GLfloat vertices[] = {
                x, y, 0.0f // 点的坐标
        };

        // 激活着色器程序
        glUseProgram(this->shaderProgram_);

        // 绑定顶点缓冲区
        glGenBuffers(1, &this->vertexBufferID_);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // 启用顶点属性
        GLuint posAttrib = glGetAttribLocation(this->shaderProgram_, "pos");
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(posAttrib);

        // 设置点的颜色（可以通过 Uniform 来设置颜色）
        GLint colorLoc = glGetUniformLocation(this->shaderProgram_, "color");
        glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); // 绿色

        // 绘制点
        glDrawArrays(GL_POINTS, 0, 1); // 使用 GL_POINTS 绘制单个点

        return *this;
    }

    Graphics &Graphics::addText(const std::string &text, const glm::ivec2 &pos, const glm::ivec4 &color, const bool isCenterBottom) {

        // SDL_SetRenderDrawColor(this->renderer_, 0, 0, 255, 255);
        // TODO: Implement Graphics::addText()
        // SDL_RenderDebugText(this->renderer_, (float)pos.x, (float)pos.y, text.c_str());

        // 激活着色器程序
    glUseProgram(this->shaderProgram_);

    // 绑定字体纹理（假设已通过其他方式加载）
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->font_texture_); // 需要提前加载字体纹理

    // 设置纹理 uniform
    GLint texUniform = glGetUniformLocation(this->shaderProgram_, "textureSampler");
    glUniform1i(texUniform, 0);

    // 设置颜色 uniform（用于颜色调制）
    GLint colorLoc = glGetUniformLocation(this->shaderProgram_, "color");
    glUniform4f(colorLoc,
        static_cast<float>(color.r)/255.0f,
        static_cast<float>(color.g)/255.0f,
        static_cast<float>(color.b)/255.0f,
        static_cast<float>(color.a)/255.0f
    );

    // 设置投影矩阵（同 addRect）
    float left = 0.0f;
    float right = static_cast<float>(this->options_.size.x);
    float bottom = static_cast<float>(this->options_.size.y);
    float top = 0.0f;
    glm::mat4 projection = glm::ortho(left, right, bottom, top);
    GLint projectionLoc = glGetUniformLocation(this->shaderProgram_, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // 设置模型矩阵（同 addRect）
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(this->options_.scale.x, this->options_.scale.y, 1.0f));
    GLint modelLoc = glGetUniformLocation(this->shaderProgram_, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // 计算初始位置（考虑居中底部对齐）
    float baseX = static_cast<float>(pos.x);
    float baseY = static_cast<float>(pos.y);
    if (isCenterBottom) {
        // 计算文本总宽度（假设字符逻辑宽度为1，实际尺寸由 scale 控制）
        float totalWidth = text.length() * this->options_.scale.x;
        baseX -= totalWidth / 2.0f;
        baseY -= this->options_.scale.y; // 字符逻辑高度为1
    }

    // 字符尺寸（逻辑单位，实际尺寸由 model 矩阵的 scale 控制）
    const float CHAR_WIDTH = 1.0f;
    const float CHAR_HEIGHT = 1.0f;
    float cursorX = baseX;
    float cursorY = baseY;

    // 遍历每个字符
    for (char c : text) {
        // 跳过非打印字符（ASCII 32~127）
        if (c < 32 || c > 127) {
            cursorX += CHAR_WIDTH; // 保持间距一致
            continue;
        }

        // 计算 UV 坐标（16x16 网格）
        const int ASCII_START = 32;
        int asciiIndex = static_cast<int>(c) - ASCII_START;
        int col = asciiIndex % 16;
        int row = 15 - (asciiIndex / 16); // 翻转行号（假设纹理原点在左下）

        float uStep = 1.0f / 16.0f;
        float vStep = 1.0f / 16.0f;

        float uStart = col * uStep;
        float uEnd = (col + 1) * uStep;
        float vStart = row * vStep;
        float vEnd = (row + 1) * vStep;

        // 顶点数据（位置 + UV）
        GLfloat vertices[] = {
            // 位置          UV
            cursorX,          cursorY,          0.0f, uStart, vEnd,   // 左下
            cursorX + CHAR_WIDTH, cursorY,          0.0f, uEnd,   vEnd,   // 右下
            cursorX + CHAR_WIDTH, cursorY + CHAR_HEIGHT, 0.0f, uEnd,   vStart, // 右上
            cursorX,          cursorY + CHAR_HEIGHT, 0.0f, uStart, vStart  // 左上
        };

        // 更新顶点缓冲区
        glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // 设置顶点属性
        // 位置属性
        GLuint posAttrib = glGetAttribLocation(this->shaderProgram_, "pos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);

        // 纹理坐标属性
        GLuint texAttrib = glGetAttribLocation(this->shaderProgram_, "texCoord");
        glEnableVertexAttribArray(texAttrib);
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

        // 绘制字符
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // 移动光标到下一个字符位置
        cursorX += CHAR_WIDTH;
    }

    // 清理状态
    glDisableVertexAttribArray(glGetAttribLocation(this->shaderProgram_, "pos"));
    glDisableVertexAttribArray(glGetAttribLocation(this->shaderProgram_, "texCoord"));

    return *this;

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
    GLuint Graphics::loadFontTexture(const std::string& fontPath) {
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
