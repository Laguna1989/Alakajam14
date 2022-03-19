#include "spell_passive_movement_speed.hpp"
#include "state_game.hpp"

SpellPassiveMovementSpeed::SpellPassiveMovementSpeed(CharacterSheetImgui& sheet)
    : SpellPassiveImpl(sheet)
{
}

void SpellPassiveMovementSpeed::onEquip() { m_charSheet.setMovementSpeedFactor("Spell", 0.125f); }
void SpellPassiveMovementSpeed::onUnEquip() { m_charSheet.setMovementSpeedFactor("Spell", 0.0f); }
std::string SpellPassiveMovementSpeed::getName() const
{
    return "Passive: Improved Movement Speed";
}
