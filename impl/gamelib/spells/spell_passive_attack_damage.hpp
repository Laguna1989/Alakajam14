#ifndef ALAKAJAM14_SPELL_PASSIVE_ATTACK_DAMAGE_HPP
#define ALAKAJAM14_SPELL_PASSIVE_ATTACK_DAMAGE_HPP

#include "spell_impl.hpp"

class SpellPassiveAttackDamage : public SpellPassiveImpl {
public:
    explicit SpellPassiveAttackDamage(CharacterSheetImgui& sheet);
    void onEquip() override;
    void onUnEquip() override;
    std::string getName() const override;
};

#endif // ALAKAJAM14_SPELL_PASSIVE_ATTACK_DAMAGE_HPP
