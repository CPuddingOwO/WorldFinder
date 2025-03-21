#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "drawlist.hpp"
#include "WorldFinder/Error.hpp"

namespace wf::game::render {
    using namespace wf::err;
    class Adapter {
    public:
        virtual Result Initialize()  = 0;   // 初始化渲染器
        virtual Result Submit(const Drawlist& drawlist) = 0;        // 提交绘制命令
        virtual Result Reload()     = 0;    // 重载渲染器
        virtual Result Destroy()     = 0;   // 销毁渲染器

        virtual ~Adapter() = default;       // 默认析构
    };
}
