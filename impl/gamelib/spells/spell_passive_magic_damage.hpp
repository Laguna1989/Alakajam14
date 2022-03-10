#ifndef ALAKAJAM14_SPELL_PASSIVE_MAGIC_DAMAGE_HPP
#define ALAKAJAM14_SPELL_PASSIVE_MAGIC_DAMAGE_HPP

#include "spell_impl.hpp"

class SpellPassiveMagicDamage : public SpellPassiveImpl {
public:
    explicit SpellPassiveMagicDamage(CharacterSheetImgui& sheet);
    void onEquip() override;
    void onUnEquip() override;
    std::string getName() const override;
};

#endif // ALAKAJAM14_SPELL_PASSIVE_MAGIC_DAMAGE_HPP
