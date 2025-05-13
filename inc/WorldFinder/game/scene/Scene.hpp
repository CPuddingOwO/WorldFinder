#pragma once
#include <WorldFinder/Error.hpp>
using namespace wf::err;

namespace wf::game::scene {
    class Scene {
    // Do
    public:
        virtual Result Initialize() = 0;
        virtual Result Destroy() = 0;
        virtual ~Scene() = default;
    };
}