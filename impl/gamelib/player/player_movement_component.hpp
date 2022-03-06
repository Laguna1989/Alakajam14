#ifndef ALAKAJAM14_PLAYER_MOVEMENT_COMPONENT_HPP
#define ALAKAJAM14_PLAYER_MOVEMENT_COMPONENT_HPP

#include "input/input_manager_interface.hpp"
#include "input_component_interface.hpp"
#include <memory>

class PlayerInputComponent : public InputComponentInterface {
public:
    explicit PlayerInputComponent(std::shared_ptr<jt::KeyboardInputInterface> keyboardInterface);

    void updateMovement(Player& player) override;
    void updateAttack(Player& player) override;

private:
    std::shared_ptr<jt::KeyboardInputInterface> m_keyboard { nullptr };
};

#endif // ALAKAJAM14_PLAYER_MOVEMENT_COMPONENT_HPP
