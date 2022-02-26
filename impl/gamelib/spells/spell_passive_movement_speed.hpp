#ifndef ALAKAJAM14_SPELL_PASSIVE_MOVEMENT_SPEED_HPP
#define ALAKAJAM14_SPELL_PASSIVE_MOVEMENT_SPEED_HPP

#include "spell_impl.hpp"
#include <string>

class SpellPassiveMovementSpeed : public SpellPassiveImpl {
public:
    explicit SpellPassiveMovementSpeed(StateGame& state);
    int getExperienceCost() const override;
    void update(float elapsed) override;

    void onEquip() override;
    void onUnEquip() override;

    std::string getName() const override;

private:
    void doTrigger() override;
};

#endif // ALAKAJAM14_SPELL_PASSIVE_MOVEMENT_SPEED_HPP
