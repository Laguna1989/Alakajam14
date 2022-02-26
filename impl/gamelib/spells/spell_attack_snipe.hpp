#ifndef ALAKAJAM14_SPELL_ATTACK_SNIPE_HPP
#define ALAKAJAM14_SPELL_ATTACK_SNIPE_HPP

#include "spell_impl.hpp"

class SpellAttackSnipe : public SpellTriggerImpl {
public:
    // TODO make this class only depend on splitted interfaces, not on complete StateGame.
    explicit SpellAttackSnipe(StateGame&);
    void doTrigger() override;
    int getExperienceCost() const override;
    std::string getName() const override;
};

#endif // ALAKAJAM14_SPELL_ATTACK_SNIPE_HPP
