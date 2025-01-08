#pragma once

#include <stdexcept>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <format>
#include <typeindex>

namespace di {

    class ServiceLocator {
    public:
        template <typename T>
        void Provide(const std::shared_ptr<T>& service) {
            std::lock_guard<std::mutex> lock(mutex_);
            registry[typeid(T)] = service;
        }

        template <typename T>
        void Provide(T* service) {
            std::lock_guard<std::mutex> lock(mutex_);
            registry[typeid(T)] = std::shared_ptr<T>(service);
        }

        template <typename T>
        std::shared_ptr<T> Get() {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = registry.find(typeid(T));
            if (it != registry.end()) {
                return std::static_pointer_cast<T>(it->second);
            }
            throw std::runtime_error(std::format("Service not found: {}", typeid(T).name()));
        }

        void Destroy() {
            std::lock_guard<std::mutex> lock(mutex_);
            registry.clear();
        }

    private:
        static inline std::unordered_map<std::type_index, std::shared_ptr<void>> registry;
        static inline std::mutex mutex_;
    };

} // namespace di
