#ifndef ALAKAJAM14_PLAYER_MOVEMENT_COMPONENT_HPP
#define ALAKAJAM14_PLAYER_MOVEMENT_COMPONENT_HPP

#include "movement_component_interface.hpp"
#include "input/input_manager_interface.hpp"
#include <memory>

class PlayerMovementComponent : public MovementComponentInterface {
public:
    explicit PlayerMovementComponent(std::shared_ptr<jt::KeyboardInputInterface> keyboardInterface);

    void update(Player& player) override;

private:

};

#endif // ALAKAJAM14_PLAYER_MOVEMENT_COMPONENT_HPP
