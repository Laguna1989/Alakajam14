#ifndef ALAKAJAM14_SPELL_ATTACK_BROAD_HPP
#define ALAKAJAM14_SPELL_ATTACK_BROAD_HPP

#include "spell_impl.hpp"
#include <string>

class SpellAttackBroad : public SpellTriggerImpl {
public:
    SpellAttackBroad(StateGame& state);
    int getExperienceCost() const override;
    std::string getName() const override;

private:
    void doTrigger() override;
};

#endif // ALAKAJAM14_SPELL_ATTACK_BROAD_HPP
