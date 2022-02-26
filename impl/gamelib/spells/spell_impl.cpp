#include "spell_impl.hpp"

SpellImpl::SpellImpl(StateGame& state)
    : m_state { state }
{
}
void SpellImpl::update(float elapsed) { m_cooldown -= elapsed; }

bool SpellImpl::canTrigger() const { return m_cooldown <= 0.0f; }

void SpellImpl::trigger()
{
    if (canTrigger()) {
        doTrigger();
    }
}
