#pragma once

#include <stdexcept>
#include <unordered_map>
#include <memory>
#include <format>
#include <typeindex>

namespace di {
    class DependencyInjector {
    public:
        template <typename T>
        void RegisterDependency(std::shared_ptr<T> dependency) {
            auto typeIndex = std::type_index(typeid(T));
            dependencies[typeIndex] = std::static_pointer_cast<void>(dependency);
        }

        template <typename T>
        std::shared_ptr<T> GetDependency() const {
            auto typeIndex = std::type_index(typeid(T));
            auto it = dependencies.find(typeIndex);
            if (it != dependencies.end()) {
                return std::static_pointer_cast<T>(it->second);
            }
            throw std::runtime_error(std::format("Dependency not found: {}", typeid(T).name()));
        }

    private:
        mutable std::unordered_map<std::type_index, std::shared_ptr<void>> dependencies;
    };

} // namespace di
