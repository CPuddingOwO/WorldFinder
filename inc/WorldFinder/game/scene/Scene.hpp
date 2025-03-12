#pragma once
#include <WorldFinder/Error.hpp>
using namespace wf::err;

namespace wf::game::scene {
    class Scene {
    // Do
    public:
        virtual ~Scene() = default;
    };
}