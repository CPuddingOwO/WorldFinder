#include <algorithm>
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <WorldFinder/game/render/adapter/SDLRendererAdapter.hpp>

namespace wf::game::render {
    SDLRendererAdapter::SDLRendererAdapter(const SDLRendererAdapterOption& op): window(nullptr), renderer(nullptr) {
        this->option = op;
    }

    SDLRendererAdapter::~SDLRendererAdapter() {
        Destroy();
    }

    Result SDLRendererAdapter::Initialize() {
        this->window = CreateSDLWindow(this->option);
        this->renderer = CreateSDLRenderer(this->window, this->option);

        return Result();
    }

    Result SDLRendererAdapter::Submit(const Drawlist& drawlist) {
        auto vertices = drawlist.GetVertices();
        auto indices = drawlist.GetIndices();
        const auto& cmds = drawlist.GetCommands();

        SDL_SetRenderDrawColor(this->renderer, 88, 129, 87, 255 );
        // SDL_SetRenderDrawColor(this->renderer, 88, 129, 198, 255 );
        SDL_RenderClear(this->renderer);

        // spdlog::info("CmdSize: {}", cmds.size());
        for (const auto& cmd : cmds) {
            std::vector<SDL_Vertex> vtx;
            vtx.resize(cmd.vtxCount);
            // spdlog::trace("ElemCount: {}", cmd.elemCount);
            // spdlog::trace("VCount: {}, VOffset: {}", cmd.vtxCount, cmd.vtxOffset);

            // std::memcpy(vtx.data(), &vertices + cmd.vtxOffset, cmd.vtxCount * sizeof(SDL_Vertex));
            // 使用 std::transform 转换数据
            std::transform(vertices.begin() + cmd.vtxOffset, vertices.begin() + cmd.vtxOffset + cmd.vtxCount,
                           vtx.begin(), [](const Vertex& src) {
                               return SDL_Vertex{
                                   {src.pos.x, src.pos.y},  // 位置
                                   {static_cast<float>(src.col.r) / 255.0f, static_cast<float>(src.col.g) / 255.0f, static_cast<float>(src.col.b) / 255.0f, static_cast<float>(src.col.a) / 255.0f},  // 颜色
                                   {src.uv.x, src.uv.y}  // UV坐标
                               };
                           });
            // for (const auto& v : vtx) {
            //     // spdlog::trace("Vert: ({}, {}) Uv: ({}, {})", v.pos.x, v.pos.y, v.uv.x, v.uv.y);
            //     spdlog::trace("Vert: ({}, {}) Uv: ({}, {}) Col: ({}, {}, {}, {})", v.position.x, v.position.y, v.tex_coord.x, v.tex_coord.y, v.color.r, v.color.g, v.color.b, v.color.a);
            //     spdlog::trace("Index: ({}), Offset: {}", indices.size(), cmd.idxOffset);
            // }
            if (!SDL_RenderGeometry(this->renderer, nullptr, vtx.data(), static_cast<int>(cmd.vtxCount), reinterpret_cast<const int *>(indices.data()), cmd.elemCount)) {
                spdlog::error("Error: SDL_RenderGeometry(): {}", SDL_GetError());
            }
        }

        SDL_RenderPresent(this->renderer);
        return Result();
    }

    Result SDLRendererAdapter::Reload() {
        DestroySDLRenderer(*this->renderer);
        this->renderer = CreateSDLRenderer(this->window, this->option);
        return Result();
    }

    Result SDLRendererAdapter::Destroy() {
        DestroySDLRenderer(*this->renderer);
        DestroySDLWindow(*this->window);
        return Result();
    }

    SDL_Window* SDLRendererAdapter::CreateSDLWindow(const SDLRendererAdapterOption &op) {
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

    SDL_Renderer* SDLRendererAdapter::CreateSDLRenderer(SDL_Window *wind, const SDLRendererAdapterOption &op) {
        SDL_PropertiesID props = SDL_CreateProperties();
        // SDL_SetStringProperty(props, SDL_PROP_RENDERER_CREATE_NAME_STRING, "Vulkan");
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
}
