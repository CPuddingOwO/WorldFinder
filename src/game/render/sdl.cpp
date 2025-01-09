#include <WorldFinder/game/render/sdl.hpp>

namespace game::render::sdl {
    Graphics::Graphics(const std::shared_ptr<di::DependencyInjector>& injector, const GraphicsOptions& options) {
        this->options = options;

        this->window = createSDLWindow(options);
        this->renderer = createSDLRenderer(this->window, options);
    }

    Graphics::~Graphics() {
        destroySDLWindow(*this->window);
        destroySDLRenderer(*this->renderer);
    }

    SDL_Window* Graphics::createSDLWindow(const GraphicsOptions& op) {
        SDL_PropertiesID props = SDL_CreateProperties();
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

    Graphics &Graphics::addRect(const glm::ivec2 &pos, const glm::ivec2 &size, bool isCentered) {
        SDL_FRect rect = {(float)pos.x, (float)pos.y, (float)size.x, (float)size.y};
        if (isCentered) {
            rect.x -= (float)size.x / 2;
            rect.y -= (float)size.y / 2;
        }
        SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
        SDL_RenderRect(this->renderer, &rect);
        return *this;
    }

    void Graphics::onRender() {
        SDL_RenderPresent(this->renderer);
        SDL_SetRenderDrawColor(this->renderer, 88, 129, 87, 255 );
        SDL_RenderClear(this->renderer);
    }
}