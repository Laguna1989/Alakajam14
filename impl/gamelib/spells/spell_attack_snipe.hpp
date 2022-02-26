#ifndef ALAKAJAM14_SPELL_ATTACK_SNIPE_HPP
#define ALAKAJAM14_SPELL_ATTACK_SNIPE_HPP

#include "spell_impl.hpp"
#include <string>

class SpellAttackSnipe : public SpellTriggerImpl {
public:
    explicit SpellAttackSnipe(StateGame&);
    void doTrigger() override;
    int getExperienceCost() const override;
    std::string getName() const override;
};

#endif // ALAKAJAM14_SPELL_ATTACK_SNIPE_HPP
