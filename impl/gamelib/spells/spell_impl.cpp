#include "spell_impl.hpp"

SpellTriggerImpl::SpellTriggerImpl(StateGame& state)
    : m_state { state }
{
}
void SpellTriggerImpl::update(float elapsed) { m_cooldown -= elapsed; }

bool SpellTriggerImpl::canTrigger() const { return m_cooldown <= 0.0f; }

void SpellTriggerImpl::trigger()
{
    if (canTrigger()) {
        doTrigger();
    }
}

void SpellTriggerImpl::onEquip() { }
void SpellTriggerImpl::onUnEquip() { }

SpellPassiveImpl::SpellPassiveImpl(CharacterSheetImgui& charSheet)
    : m_charSheet { charSheet }
{
}

void SpellPassiveImpl::trigger() { }
bool SpellPassiveImpl::canTrigger() const { return false; }
int SpellPassiveImpl::getExperienceCost() const { return 0; }
void SpellPassiveImpl::update(float elapsed) { }
