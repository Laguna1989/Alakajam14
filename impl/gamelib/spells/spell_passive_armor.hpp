#ifndef ALAKAJAM14_SPELL_PASSIVE_ARMOR_HPP
#define ALAKAJAM14_SPELL_PASSIVE_ARMOR_HPP

#include "spell_impl.hpp"

class SpellPassiveArmor : public SpellPassiveImpl {
public:
    SpellPassiveArmor(CharacterSheetImgui& sheet);
    void onEquip() override;
    void onUnEquip() override;
    std::string getName() const override;
};

#endif // ALAKAJAM14_SPELL_PASSIVE_ARMOR_HPP
