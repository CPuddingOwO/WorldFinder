#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace wf::game::render {
    // using Color = glm::u8vec4;
    using Pos   = glm::vec<2, float>;
    using Uv    = glm::vec<2, float>;
    using Color = glm::vec<4, int>;

    struct Vertex {
        Pos pos;
        Uv uv;
        Color col;

        explicit Vertex(Pos p, Uv u, Color c) : pos(p), uv(u), col(c) {}
    };

}