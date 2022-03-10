#include "spell_passive_magic_damage.hpp"

SpellPassiveMagicDamage::SpellPassiveMagicDamage(CharacterSheetImgui& sheet)
    : SpellPassiveImpl(sheet)
{
}

void SpellPassiveMagicDamage::onEquip() { m_charSheet.setMagicDamageFactor("spell", 15.0f); }
void SpellPassiveMagicDamage::onUnEquip() { m_charSheet.setMagicDamageFactor("spell", 0.0f); }
std::string SpellPassiveMagicDamage::getName() const { return std::string(); }
