#include <WorldFinder/game/entity/Player.hpp>

namespace wf::game::entity {
    Player::Player(std::string name, flecs::world* world) {
        this->name = std::move(name);
        this->entity = world->entity(name.c_str());
        this->entity.set<Collision>({32, 32});
    }

    void Player::init_component() {
        this->init_move_component();
        this->init_physics_component();
    }
    void Player::jump() {
        if (!is_touch_floor()) return;
        auto vel = this->entity.get<Velocity>();
        this->entity.set<Velocity>({vel->x, vel->y - 0.75});
    }
    void Player::go_left() {
        auto vel = this->entity.get<Velocity>();
        this->entity.set<Velocity>({vel->x - 0.25, vel->y});
    }
    void Player::go_right() {
        auto vel = this->entity.get<Velocity>();
        this->entity.set<Velocity>({vel->x + 0.25, vel->y});
    }

    bool Player::is_touch_floor() {
        return this->entity.get<Physical>()->is_touch_floor;
    }

    bool Player::is_touch_wall() {
        return this->entity.get<Physical>()->is_touch_wall;
    }

    bool Player::is_touch_ceiling() {
        return this->entity.get<Physical>()->is_touch_ceiling;
    }

    bool Player::will_touch_floor() {
        return this->entity.get<Physical>()->will_touch_floor;
    }

    void Player::init_physics_component() {
        this->entity.set<Physical>({true, 0.25});
    }
    void Player::init_move_component() {
        this->entity.set<Velocity>({0, 0});
        this->entity.set<Position>({64, 16, 0});
    }

}