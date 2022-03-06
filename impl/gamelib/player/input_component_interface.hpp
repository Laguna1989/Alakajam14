#ifndef ALAKAJAM14_INPUT_COMPONENT_INTERFACE_HPP
#define ALAKAJAM14_INPUT_COMPONENT_INTERFACE_HPP

class Player;

class InputComponentInterface {
public:
    virtual ~InputComponentInterface() = default;

    virtual void updateMovement(Player& player) = 0;
    virtual void updateAttack(Player& player) = 0;
    virtual void updateSpells(Player& player) = 0;
};

#endif // ALAKAJAM14_INPUT_COMPONENT_INTERFACE_HPP
