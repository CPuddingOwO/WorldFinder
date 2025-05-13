#pragma once
#include <glm/vec2.hpp>

namespace wf::game::scene {
    template <typename T = float>
    struct Tile {
        int id;
        glm::vec<2, T> pos, size;

        Tile(const int id, const T x, const T y, const T width, const T height) :
            id(id), pos(x, y), size(width, height) {
        }
        Tile(const int id, const glm::vec<2, T>& pos, const glm::vec<2, T>& size) :
            id(id), pos(pos), size(size) {
        }
    };
}