#include "spell_heal_large.hpp"
#include <algorithm>

SpellHealLarge::SpellHealLarge(CharacterSheetImgui& charSheet)
    : m_charSheet { charSheet }
{
}

void SpellHealLarge::trigger()
{
    m_cooldown += 0.5f;
    float amountToHeal
        = std::min(m_charSheet.getHitpointsMax() - m_charSheet.getHitpoints(), 55.0f);
    m_charSheet.changeHitpoints(-amountToHeal);
}

bool SpellHealLarge::canTrigger() const
{
    bool const hasLessThanFullHitpoints
        = m_charSheet.getHitpoints() < m_charSheet.getHitpointsMax();
    return m_cooldown <= 0.0f && hasLessThanFullHitpoints;
}
void SpellHealLarge::update(float elapsed) { m_cooldown -= elapsed; }
int SpellHealLarge::getExperienceCost() const { return 35; }
void SpellHealLarge::onEquip() { }
void SpellHealLarge::onUnEquip() { }
std::string SpellHealLarge::getName() const { return "Rejuvenate"; }
