#pragma once
#include <vector>
#include "types.hpp"

namespace wf::game::render {
    struct DrawCommand {
        uint32_t elemCount;    // 索引数量
        uint32_t idxOffset;    // 索引缓冲的起始位置
        uint32_t vtxOffset;    // 顶点缓冲的起始位置
        uint32_t vtxCount;     // 顶点数量（用于合并检查）
        void* textureId;       // 纹理标识符

        DrawCommand(const uint32_t e, const uint32_t io, const uint32_t vo, void* tex, const uint32_t vc)
            : elemCount(e), idxOffset(io), vtxOffset(vo), vtxCount(vc), textureId(tex) {}
    };

    class Drawlist {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<DrawCommand> drawCommands;

        // 当前未提交批次的起始位置
        uint32_t currentBatchIdxOffset = 0;
        uint32_t currentBatchVtxOffset = 0;

    public:
        // 添加矩形（生成4顶点，6索引）
        void AddRect(const glm::vec<2, float>& min, const glm::vec<2, float>& max, Color col, const glm::vec<2, float>& uv_min = {}, const glm::vec<2, float>& uv_max = {});


        // 提交当前批次为一个绘制命令
        void SubmitDrawCommand(void* texture_id);
        void Clear(); /// 清空

        [[nodiscard]] const std::vector<Vertex>& GetVertices() const { return vertices; }
        [[nodiscard]] const std::vector<uint32_t>& GetIndices() const { return indices; }
        [[nodiscard]] const std::vector<DrawCommand>& GetCommands() const { return drawCommands; }
    };
}
