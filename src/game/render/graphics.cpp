#include <fstream>
#include <WorldFinder/game/render/graphics.hpp>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "WorldFinder/game/render/adapter/SDLRendererAdapter.hpp"


namespace wf::game::render::gfx {
    Graphics::Graphics(const std::shared_ptr<di::DependencyInjector>& injector, const GraphicsOptions& options) {
        this->options = options;
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            throw std::runtime_error("Error: SDL_Init(): " + std::string(SDL_GetError()));
        }
        this->drawlist = std::make_shared<Drawlist>();
        auto ca = SDLRendererAdapterOption(options.title, options.isVsync, options.isBorderless, options.size);
        this->adapter = std::make_shared<SDLRendererAdapter>(ca);
        adapter->Initialize();
    }

    Graphics::~Graphics() {
        this->adapter->Destroy();
        SDL_Quit();
    }

    Graphics &Graphics::setVSync(const bool isVsync) {
        this->options.isVsync = isVsync;
        SDL_GL_SetSwapInterval( isVsync ? 1 : 0);
        return *this;
    }

    Graphics &Graphics::setBorderless(bool isBorderless) {
        this->options.isBorderless = isBorderless;
        // SDL_SetWindowBordered(this->window, !isBorderless);
        return *this;
    }

    Graphics& Graphics::setScale(float x, float y) {
        this->options.scale.x = x;
        this->options.scale.y = y;

        return *this;
    }

    Graphics& Graphics::addRect(const glm::ivec2& pos, const glm::ivec2& size, const glm::ivec4& color, bool fill, bool center_bottom) {
        if (center_bottom) {
            glm::ivec2 adjustedPos = {pos.x - size.x / 2, pos.y - size.y};
            this->drawlist->AddRect(adjustedPos, adjustedPos + size, color, {}, {});
        } else {
            this->drawlist->AddRect(pos, pos + size, color, {}, {});
        }
        return *this;
    }

//     Graphics &Graphics::addRect(const glm::ivec2 &pos, const glm::ivec2 &size, const glm::ivec4& color, bool isCenterBottom) {
//
// //        SDL_FRect rect = {(float)pos.x, (float)pos.y, (float)size.x, (float)size.y};
// //        if (isCenterBottom) {
// //            rect.x -= (float)size.x / 2;
// //            rect.y -= (float)size.y;
// //        }
// //        SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
// //        SDL_RenderRect(this->renderer, &rect);
// //        return *this;
//
//         // 计算矩形的顶点位置
//         auto x = (float)pos.x;
//         auto y = (float)pos.y;
//         auto width = (float)size.x;
//         auto height = (float)size.y;
//
//         if (isCenterBottom) {
//             x -= width / 2.0f;
//             y -= height;
//         }
//
//         // 定义矩形的 4 个顶点
//         GLfloat vertices[] = {
//                 x, y, 0.0f,                  // 左下角
//                 x + width, y, 0.0f,          // 右下角
//                 x + width, y + height, 0.0f, // 右上角
//                 x, y + height, 0.0f          // 左上角
//         };
//
//         // 激活着色器程序
//         glUseProgram(this->shaderProgram);
//
//         // 绑定顶点缓冲区
//         glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//         // 启用顶点属性
//         GLuint posAttrib = glGetAttribLocation(this->shaderProgram, "pos");
//         glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);
//         glEnableVertexAttribArray(posAttrib);
//
//         // 设置投影矩阵
//         float left = 0.0f, right = (float)this->options_.size.x, bottom = (float)this->options_.size.y, top = 0.0f;
//         glm::mat4 projection = glm::ortho(left, right, bottom, top);
//         GLint projectionLoc = glGetUniformLocation(this->shaderProgram, "projection");
//         glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
//
//         // 计算或获取模型矩阵
//         auto model = glm::mat4(1.0f); // 初始为单位矩阵
//         // 如果有平移、旋转或缩放，可以依次应用变换
// //        model = glm::translate(model, glm::vec3(1.0f, 2.0f, 0.0f)); // 例如平移
// //        model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // 旋转 -5 度
//         model = glm::scale(model, glm::vec3(this->options_.scale.x, this->options_.scale.y, 1.0f)); // 缩放
//
//         // 传递模型矩阵到着色器
//         GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
//         glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); // 传递模型矩阵
//
//         // 设置颜色（这里你也可以通过 uniform 变量来设置颜色）
//         GLint colorLoc = glGetUniformLocation(this->shaderProgram, "color");
//         glUniform4f(colorLoc, (float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f, (float)color.a / 255.0f); // 纯红色
//
//         // 绘制矩形（使用两个三角形）
//         glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // 使用 GL_TRIANGLE_FAN 绘制矩形
//
//         return *this;
//     }
//
//     Graphics &Graphics::addPoint(const glm::ivec2 &pos) {
// //        SDL_SetRenderDrawColor(this->renderer_, 0, 255, 0, 255);
// //        SDL_RenderPoint(this->renderer_, (float)pos.x, (float)pos.y);
// //        return *this;
//         // 将屏幕坐标转换为标准化设备坐标
//         //
//         float x = (2.0f * (float)pos.x) / (float)this->options_.size.x - 1.0f;
//         float y = 1.0f - (2.0f * (float)pos.y) / (float)this->options_.size.y;
//
//         // 创建一个包含点位置的顶点数组
//         GLfloat vertices[] = {
//                 x, y, 0.0f // 点的坐标
//         };
//
//         // 激活着色器程序
//         glUseProgram(this->shaderProgram_);
//
//         // 绑定顶点缓冲区
//         glGenBuffers(1, &this->vertexBufferID_);
//         glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID_);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//         // 启用顶点属性
//         GLuint posAttrib = glGetAttribLocation(this->shaderProgram_, "pos");
//         glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//         glEnableVertexAttribArray(posAttrib);
//
//         // 设置点的颜色（可以通过 Uniform 来设置颜色）
//         GLint colorLoc = glGetUniformLocation(this->shaderProgram_, "color");
//         glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); // 绿色
//
//         // 绘制点
//         glDrawArrays(GL_POINTS, 0, 1); // 使用 GL_POINTS 绘制单个点
//
//         return *this;
//     }
//
//     Graphics &Graphics::addText(const std::string &text, const glm::ivec2 &pos, const glm::ivec4 &color, const bool isCenterBottom) {
//
//         // SDL_SetRenderDrawColor(this->renderer_, 0, 0, 255, 255);
//         // TODO: Implement Graphics::addText()
//         // SDL_RenderDebugText(this->renderer_, (float)pos.x, (float)pos.y, text.c_str());
//
//         // 激活着色器程序
//     glUseProgram(this->shaderProgram_);
//
//     // 绑定字体纹理（假设已通过其他方式加载）
//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, this->font_texture_); // 需要提前加载字体纹理
//
//     // 设置纹理 uniform
//     GLint texUniform = glGetUniformLocation(this->shaderProgram_, "textureSampler");
//     glUniform1i(texUniform, 0);
//
//     // 设置颜色 uniform（用于颜色调制）
//     GLint colorLoc = glGetUniformLocation(this->shaderProgram_, "color");
//     glUniform4f(colorLoc,
//         static_cast<float>(color.r)/255.0f,
//         static_cast<float>(color.g)/255.0f,
//         static_cast<float>(color.b)/255.0f,
//         static_cast<float>(color.a)/255.0f
//     );
//
//     // 设置投影矩阵（同 addRect）
//     float left = 0.0f;
//     float right = static_cast<float>(this->options_.size.x);
//     float bottom = static_cast<float>(this->options_.size.y);
//     float top = 0.0f;
//     glm::mat4 projection = glm::ortho(left, right, bottom, top);
//     GLint projectionLoc = glGetUniformLocation(this->shaderProgram_, "projection");
//     glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
//
//     // 设置模型矩阵（同 addRect）
//     glm::mat4 model = glm::mat4(1.0f);
//     model = glm::scale(model, glm::vec3(this->options_.scale.x, this->options_.scale.y, 1.0f));
//     GLint modelLoc = glGetUniformLocation(this->shaderProgram_, "model");
//     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//
//     // 计算初始位置（考虑居中底部对齐）
//     float baseX = static_cast<float>(pos.x);
//     float baseY = static_cast<float>(pos.y);
//     if (isCenterBottom) {
//         // 计算文本总宽度（假设字符逻辑宽度为1，实际尺寸由 scale 控制）
//         float totalWidth = text.length() * this->options_.scale.x;
//         baseX -= totalWidth / 2.0f;
//         baseY -= this->options_.scale.y; // 字符逻辑高度为1
//     }
//
//     // 字符尺寸（逻辑单位，实际尺寸由 model 矩阵的 scale 控制）
//     const float CHAR_WIDTH = 1.0f;
//     const float CHAR_HEIGHT = 1.0f;
//     float cursorX = baseX;
//     float cursorY = baseY;
//
//     // 遍历每个字符
//     for (char c : text) {
//         // 跳过非打印字符（ASCII 32~127）
//         if (c < 32 || c > 127) {
//             cursorX += CHAR_WIDTH; // 保持间距一致
//             continue;
//         }
//
//         // 计算 UV 坐标（16x16 网格）
//         const int ASCII_START = 32;
//         int asciiIndex = static_cast<int>(c) - ASCII_START;
//         int col = asciiIndex % 16;
//         int row = 15 - (asciiIndex / 16); // 翻转行号（假设纹理原点在左下）
//
//         float uStep = 1.0f / 16.0f;
//         float vStep = 1.0f / 16.0f;
//
//         float uStart = col * uStep;
//         float uEnd = (col + 1) * uStep;
//         float vStart = row * vStep;
//         float vEnd = (row + 1) * vStep;
//
//         // 顶点数据（位置 + UV）
//         GLfloat vertices[] = {
//             // 位置          UV
//             cursorX,          cursorY,          0.0f, uStart, vEnd,   // 左下
//             cursorX + CHAR_WIDTH, cursorY,          0.0f, uEnd,   vEnd,   // 右下
//             cursorX + CHAR_WIDTH, cursorY + CHAR_HEIGHT, 0.0f, uEnd,   vStart, // 右上
//             cursorX,          cursorY + CHAR_HEIGHT, 0.0f, uStart, vStart  // 左上
//         };
//
//         // 更新顶点缓冲区
//         glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferID_);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//         // 设置顶点属性
//         // 位置属性
//         GLuint posAttrib = glGetAttribLocation(this->shaderProgram_, "pos");
//         glEnableVertexAttribArray(posAttrib);
//         glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
//
//         // 纹理坐标属性
//         GLuint texAttrib = glGetAttribLocation(this->shaderProgram_, "texCoord");
//         glEnableVertexAttribArray(texAttrib);
//         glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
//
//         // 绘制字符
//         glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//
//         // 移动光标到下一个字符位置
//         cursorX += CHAR_WIDTH;
//     }
//
//     // 清理状态
//     glDisableVertexAttribArray(glGetAttribLocation(this->shaderProgram_, "pos"));
//     glDisableVertexAttribArray(glGetAttribLocation(this->shaderProgram_, "texCoord"));
//
//     return *this;
//
//         return *this;
//
//     }



}
