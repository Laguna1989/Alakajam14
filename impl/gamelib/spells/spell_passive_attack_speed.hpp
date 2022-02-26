#ifndef ALAKAJAM14_SPELL_PASSIVE_ATTACK_SPEED_HPP
#define ALAKAJAM14_SPELL_PASSIVE_ATTACK_SPEED_HPP

#include "spell_impl.hpp"
class SpellPassiveAttackSpeed : public SpellPassiveImpl {
public:
    explicit SpellPassiveAttackSpeed(CharacterSheetImgui& sheet);

    void onEquip() override;
    void onUnEquip() override;

    std::string getName() const override;
};
#endif // ALAKAJAM14_SPELL_PASSIVE_ATTACK_SPEED_HPP
