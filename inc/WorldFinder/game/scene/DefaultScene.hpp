#pragma once
#include "Scene.hpp"

namespace wf::game::scene {
    class DefaultScene final : public Scene {
    private:
        std::shared_ptr<di::DependencyInjector> injector;
    public:
        explicit DefaultScene(const std::shared_ptr<di::DependencyInjector> &injector);
        ~DefaultScene() override;
        Result Initialize() override;
        Result Destroy() override;

    };

}