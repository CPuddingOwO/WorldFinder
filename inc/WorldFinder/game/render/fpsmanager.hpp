#pragma once
#include <thread>
#include <chrono>

namespace game::render {
    class FpsManager {
    private:
        std::chrono::high_resolution_clock::time_point t1;
        std::chrono::high_resolution_clock::time_point t2;
        float dt;                   // 每帧时间（秒）
        float target_frame_time;    // 目标帧间隔（秒）
    public:
        explicit FpsManager(float target_fps) {
            target_frame_time = 1.0f / target_fps;
            dt = 0.0f;
        }
        void start() {
            t1 = std::chrono::high_resolution_clock::now();
        };
        void stop() {
            t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
            dt = duration / 1000.0f;  // 转换为秒

            // 控制帧率，确保每帧不低于目标帧时间
            if (dt < target_frame_time) {
                std::this_thread::sleep_for(std::chrono::duration<float>(target_frame_time - dt));
            }
        };

        float getDelta() { return dt; }
    private:
    };
}