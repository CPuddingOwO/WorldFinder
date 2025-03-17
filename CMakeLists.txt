cmake_minimum_required(VERSION 3.29)
project(WorldFinder)
cmake_policy(SET CMP0077 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SDL_STATIC ON)
set(SDL_SHARED OFF)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")


include(GNUInstallDirs)
list(APPEND CMAKE_MODULE_PATH "${CatBirdRemake_SOURCE_DIR}/cmake")

# add_subdirectory(...)
add_subdirectory(ext/sdl)
# find_package(...)
find_package(stb)
find_package(nlohmann_json)
find_package(flecs)
find_package(mini)
find_package(glm)
find_package(spdlog)
find_package(glad)
find_package(opengl_system)

set(SRC_CODE
        src/WorldFinder.cpp
        src/App.cpp
        inc/WorldFinder/game/ecs/world.hpp
        inc/WorldFinder/game/ecs/system.hpp
        inc/WorldFinder/game/ecs/component.hpp
        src/game/ecs/world.cpp
        src/game/ecs/system.cpp
        src/game/ecs/helper.cpp
        src/game/render/sdl.cpp
        src/game/entity/Player.cpp
        inc/WorldFinder/game/scene/Scene.hpp
        inc/WorldFinder/Error.hpp
        inc/WorldFinder/game/scene/DefaultScene.hpp
        src/game/scene/DefaultScene.cpp
        inc/WorldFinder/game/Game.hpp
        src/game/Game.cpp
        inc/WorldFinder/Input.hpp
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
        stb::stb                        # IMAGE LIB
        SDL3::SDL3-static               # GRAPHICS LIB
        nlohmann_json::nlohmann_json    # JSON LIB
        flecs::flecs_static             # ECS SYSTEM
        mini::mini                      # INI LIB
        glm::glm                        # MATH LIB
        spdlog::spdlog                  # LOG LIB
        glad::glad                      # GRAPHICS LIB
        opengl::opengl                  # GRAPHICS LIB
)