#include <WorldFinder/game/render/drawlist.hpp>

namespace wf::game::render {
    void Drawlist::SubmitDrawCommand(void *texture_id) {
        const uint32_t newIdxCount = static_cast<uint32_t>(indices.size()) - currentBatchIdxOffset;
        const uint32_t newVtxCount = static_cast<uint32_t>(vertices.size()) - currentBatchVtxOffset;

        if (newIdxCount == 0) return; // 无新增索引不提交

        // 尝试合并到上一个命令
        if (!drawCommands.empty()) {
            DrawCommand& prev = drawCommands.back();
            if (prev.textureId == texture_id &&
                prev.vtxOffset + prev.vtxCount == currentBatchVtxOffset)
            {
                prev.elemCount += newIdxCount;
                prev.vtxCount += newVtxCount;
                currentBatchIdxOffset = static_cast<uint32_t>(indices.size());
                currentBatchVtxOffset = static_cast<uint32_t>(vertices.size());
                return;
            }
        }

        // 添加新命令
        drawCommands.emplace_back(
            newIdxCount,
            currentBatchIdxOffset,
            currentBatchVtxOffset,
            texture_id,
            newVtxCount
        );

        // 更新批次起始位置
        currentBatchIdxOffset = static_cast<uint32_t>(indices.size());
        currentBatchVtxOffset = static_cast<uint32_t>(vertices.size());
    }

    void Drawlist::Clear() {
        vertices.clear();
        indices.clear();
        drawCommands.clear();
        currentBatchIdxOffset = 0;
        currentBatchVtxOffset = 0;
    }

    void Drawlist::AddRect(const glm::vec<2, float>& min, const glm::vec<2, float>& max, Color col, const glm::vec<2, float>& uv_min, const glm::vec<2, float>& uv_max) {
        const auto base_idx = static_cast<uint32_t>(vertices.size());
        vertices.emplace_back(glm::vec<2, float>{min.x, min.y}, uv_min, col); // 0
        vertices.emplace_back(glm::vec<2, float>{max.x, min.y}, glm::vec<2, float>{uv_max.x, uv_min.y}, col); // 1
        vertices.emplace_back(glm::vec<2, float>{max.x, max.y}, uv_max, col); // 2
        vertices.emplace_back(glm::vec<2, float>{min.x, max.y}, glm::vec<2, float>{uv_min.x, uv_max.y}, col); // 3

        indices.push_back(base_idx);
        indices.push_back(base_idx + 1);
        indices.push_back(base_idx + 2); // 三角形 1
        indices.push_back(base_idx);
        indices.push_back(base_idx + 2);
        indices.push_back(base_idx + 3); // 三角形 2
    }
}
