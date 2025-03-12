#pragma once
#include <WorldFinder/Error.hpp>
#include <WorldFinder/game/ecs/world.hpp>
#include <WorldFinder/game/scene/Scene.hpp>

namespace wf::game {
    using namespace wf::err;
    using namespace wf::game::scene;
    using namespace wf::game::ecs;

    enum class GameState {
        Null = -1,
        Loading = 0,
        MainMenu = 1,
        InGame = 2,
        Pause = 3,
        Quitting = 4,

    };
    struct GameOptions {
        GameState state = GameState::Null;
        Scene *scene = nullptr;
    };

    class Game {
        GameState state;
        GameOptions options;
        std::shared_ptr<di::DependencyInjector> injector{};
        std::shared_ptr<World> ecs;
    public:
        Game(const std::shared_ptr<di::DependencyInjector>& injector, const GameOptions& options);
        ~Game() {this->Destroy();};
        Result Initialize();
        Result HandleEvent();
        Result Update();
        Result Render();
        Result Destroy();
    };
}
