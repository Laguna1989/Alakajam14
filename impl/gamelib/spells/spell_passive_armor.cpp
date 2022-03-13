#include "spell_passive_armor.hpp"
SpellPassiveArmor::SpellPassiveArmor(CharacterSheetImgui& sheet)
    : SpellPassiveImpl { sheet }
{
}

void SpellPassiveArmor::onEquip() { m_charSheet.setArmorReductionValue("spell", 2.0f); }
void SpellPassiveArmor::onUnEquip() { m_charSheet.setArmorReductionValue("spell", 0.0f); }
std::string SpellPassiveArmor::getName() const { return "Passive: Improved Armor"; }
