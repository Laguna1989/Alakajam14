#include "spell_passive_movement_speed.hpp"
#include "state_game.hpp"

SpellPassiveMovementSpeed::SpellPassiveMovementSpeed(StateGame& state)
    : SpellPassiveImpl(state)
{
}

int SpellPassiveMovementSpeed::getExperienceCost() const { return 0; }
void SpellPassiveMovementSpeed::doTrigger() { }

void SpellPassiveMovementSpeed::update(float elapsed) { }

void SpellPassiveMovementSpeed::onEquip()
{
    m_state.getPlayer()->getCharSheet()->setMovementSpeedFactor("Spell", 0.5f);
}
void SpellPassiveMovementSpeed::onUnEquip()
{
    m_state.getPlayer()->getCharSheet()->setMovementSpeedFactor("Spell", 0.0f);
}
