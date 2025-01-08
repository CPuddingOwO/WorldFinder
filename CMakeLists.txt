cmake_minimum_required(VERSION 3.29)
project(WorldFinder)
#cmake_policy(SET CMP0077 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")

include(GNUInstallDirs)
list(APPEND CMAKE_MODULE_PATH "${CatBirdRemake_SOURCE_DIR}/cmake")

# add_subdirectory(...)
# find_package(...)
find_package(nlohmann_json)
find_package(flecs)
find_package(mini)
find_package(spdlog)

set(SRC_CODE
        src/WorldFinder.cpp
        src/App.cpp
        inc/WorldFinder/game/ecs/world.hpp
        inc/WorldFinder/game/ecs/system.hpp
        inc/WorldFinder/game/ecs/component.hpp
        src/game/ecs/world.cpp
        src/game/ecs/system.cpp
)

list(APPEND ${CXX_MODULES}
            src/Module/DependencyInjector.ixx
            src/Module/ServiceLocator.ixx
)


add_executable(WorldFinder ${CXX_MODULES} ${SRC_CODE})
install(TARGETS WorldFinder
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_compile_definitions( WorldFinder PRIVATE
        
)

target_include_directories(WorldFinder PUBLIC
        inc
)

target_link_libraries(WorldFinder PUBLIC
        nlohmann_json::nlohmann_json
        flecs::flecs_static
        mini::mini
        spdlog::spdlog

)
