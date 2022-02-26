#include "spell_passive_movement_speed.hpp"
#include "state_game.hpp"

SpellPassiveMovementSpeed::SpellPassiveMovementSpeed(StateGame& state)
    : SpellPassiveImpl(state)
{
}

void SpellPassiveMovementSpeed::onEquip()
{
    m_state.getPlayer()->getCharSheet()->setMovementSpeedFactor("Spell", 0.1f);
}
void SpellPassiveMovementSpeed::onUnEquip()
{
    m_state.getPlayer()->getCharSheet()->setMovementSpeedFactor("Spell", 0.0f);
}
std::string SpellPassiveMovementSpeed::getName() const { return "Increased Speed"; }
