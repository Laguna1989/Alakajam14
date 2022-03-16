#include "spell_heal_small.hpp"
#include <algorithm>

SpellHealSmall::SpellHealSmall(CharacterSheetImgui& charSheet)
    : m_charSheet { charSheet }
{
}

void SpellHealSmall::trigger()
{
    m_cooldown += 0.5f;
    float amountToHeal
        = std::min(m_charSheet.getHitpointsMax() - m_charSheet.getHitpoints(), 25.0f);
    m_charSheet.changeHitpoints(-amountToHeal);
}

bool SpellHealSmall::canTrigger() const
{
    bool const hasLessThanFullHitpoints
        = m_charSheet.getHitpoints() < m_charSheet.getHitpointsMax();
    return m_cooldown <= 0.0f && hasLessThanFullHitpoints;
}
void SpellHealSmall::update(float elapsed) { m_cooldown -= elapsed; }
int SpellHealSmall::getExperienceCost() const { return 20; }
void SpellHealSmall::onEquip() { }
void SpellHealSmall::onUnEquip() { }
std::string SpellHealSmall::getName() const { return "Mend Wounds"; }
