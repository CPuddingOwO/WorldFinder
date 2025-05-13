#pragma once
// #include <array>

#include "QuadTree.hpp"

namespace wf::game::scene {
    class TileManager {
    private:
        // bool cur = false;
        // std::array<std::shared_ptr<QuadTree>, 2> qt;
        std::shared_ptr<QuadTree> qt;
    public:
        TileManager(const glm::vec<2, float> &pos, const glm::vec<2, float> &size) {
            qt = std::make_shared<QuadTree>(pos, size);
          }
        ~TileManager() = default;

        // void swap() {
        //     cur = !cur;
        // }
        
        void insert(const Tile<float> &tile) const {
            qt->insert(tile);
        }
        
        [[nodiscard]] std::vector<Tile<float>> query(const glm::vec<2, float> &pos, const glm::vec<2, float> &size) const {
            return qt->query(pos, size);
        }
    };
}
