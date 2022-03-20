#include "spell_passive_attack_damage.hpp"
SpellPassiveAttackDamage::SpellPassiveAttackDamage(CharacterSheetImgui& sheet)
    : SpellPassiveImpl { sheet }
{
}

void SpellPassiveAttackDamage::onEquip() { m_charSheet.setAttackDamageFactor("spell", 15.0f); }
void SpellPassiveAttackDamage::onUnEquip() { m_charSheet.setAttackDamageFactor("spell", 0.0f); }

std::string SpellPassiveAttackDamage::getName() const { return "Passive: Improved Attack Damage"; }
