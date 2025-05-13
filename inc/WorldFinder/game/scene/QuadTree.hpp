#pragma once

#include <vector>
#include <memory>
#include <stack>

#include "Tiles.hpp"

namespace wf::game::scene {

    class QuadTree {
    private:
        struct Node {
            glm::vec<2, float> pos, size;
            std::vector<Tile<float>> tiles;
            std::unique_ptr<Node> topLeft, topRight, bottomLeft, bottomRight;

            explicit Node(const float x, const float y, const float width, const float height) :
                pos(x, y), size(width, height) {
            }

            explicit Node(const glm::vec<2, float> &pos, const glm::vec<2, float> &size) :
                pos(pos), size(size) {
            }

        };

        std::unique_ptr<Node> root;
        int maxTilesPerNode;
        int maxDepth;

        static void subdivide(Node &node) {
            float halfWidth = node.size.x / 2.0f;
            float halfHeight = node.size.y / 2.0f;

            node.topLeft = std::make_unique<Node>(node.pos.x, node.pos.y, halfWidth, halfHeight);
            node.topRight = std::make_unique<Node>(node.pos.x + halfWidth, node.pos.y, halfWidth, halfHeight);
            node.bottomLeft = std::make_unique<Node>(node.pos.x, node.pos.y + halfHeight, halfWidth, halfHeight);
            node.bottomRight = std::make_unique<Node>(node.pos.x + halfWidth, node.pos.y + halfHeight, halfWidth, halfHeight);
        }

        void insert(Node &node, const Tile<float> &tile, int depth) const {
            std::stack<std::tuple<Node *, const Tile<float>, int>> stack;
            stack.emplace(&node, tile, depth);

            while (!stack.empty()) {
                auto [currentNode, currentTile, currentDepth] = stack.top();
                stack.pop();

                if (currentDepth >= maxDepth || (currentNode->tiles.size() < maxTilesPerNode && !currentNode->topLeft)) {
                    currentNode->tiles.push_back(currentTile);
                    continue;
                }

                if (!currentNode->topLeft) {
                    subdivide(*currentNode);
                }

                if (currentTile.pos.x < currentNode->pos.x + currentNode->size.x / 2.0f) {
                    if (currentTile.pos.y < currentNode->pos.y + currentNode->size.y / 2.0f) {
                        stack.emplace(currentNode->topLeft.get(), currentTile, currentDepth + 1);
                    } else {
                        stack.emplace(currentNode->bottomLeft.get(), currentTile, currentDepth + 1);
                    }
                } else {
                    if (currentTile.pos.y < currentNode->pos.y + currentNode->size.y / 2.0f) {
                        stack.emplace(currentNode->topRight.get(), currentTile, currentDepth + 1);
                    } else {
                        stack.emplace(currentNode->bottomRight.get(), currentTile, currentDepth + 1);
                    }
                }
            }
        }

        static void query(const Node &node, const float x, const float y, const float width, const float height, std::vector<Tile<float>> &result) {
            std::stack<const Node *> stack;
            stack.push(&node);

            while (!stack.empty()) {
                const Node *current = stack.top();
                stack.pop();

                if (x + width < current->pos.x || x > current->pos.x + current->size.x ||
                    y + height < current->pos.y || y > current->pos.y + current->size.y) {
                    continue;
                    }

                for (const auto &tile : current->tiles) {
                    if (tile.pos.x < x + width && tile.pos.x + tile.size.x > x &&
                        tile.pos.y < y + height && tile.pos.y + tile.size.y > y) {
                        result.push_back(tile);
                        }
                }

                if (current->topLeft) {
                    stack.push(current->topLeft.get());
                    stack.push(current->topRight.get());
                    stack.push(current->bottomLeft.get());
                    stack.push(current->bottomRight.get());
                }
            }
        }

    public:
        QuadTree(float x, float y, float width, float height, const int maxTilesPerNode = 4, const int maxDepth = 5) :
            root(std::make_unique<Node>(x, y, width, height)), maxTilesPerNode(maxTilesPerNode), maxDepth(maxDepth) {
        }
        QuadTree(const glm::vec<2, float> &pos, const glm::vec<2, float> &size, const int maxTilesPerNode = 4, const int maxDepth = 5) :
            root(std::make_unique<Node>(pos, size)), maxTilesPerNode(maxTilesPerNode), maxDepth(maxDepth) {
        }

        void insert(const Tile<float> &tile) {
            insert(*root, tile, 0);
        }

        [[nodiscard]] std::vector<Tile<float>> query(const float x, const float y, const float width, const float height) const {
            std::vector<Tile<float>> result;
            query(*root, x, y, width, height, result);
            return result;
        }
        [[nodiscard]] std::vector<Tile<float>> query(const glm::vec<2, float> &pos, const glm::vec<2, float> &size) const {
            std::vector<Tile<float>> result;
            query(*root, pos.x, pos.y, size.x, size.y, result);
            return result;
        }
    };

}