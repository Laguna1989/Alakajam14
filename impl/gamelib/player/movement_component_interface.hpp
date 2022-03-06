#ifndef ALAKAJAM14_MOVEMENT_COMPONENT_INTERFACE_HPP
#define ALAKAJAM14_MOVEMENT_COMPONENT_INTERFACE_HPP

class Player;

class MovementComponentInterface {
public:
    virtual ~MovementComponentInterface() = default;

    virtual void update(Player& player) = 0;
};

#endif // ALAKAJAM14_MOVEMENT_COMPONENT_INTERFACE_HPP
