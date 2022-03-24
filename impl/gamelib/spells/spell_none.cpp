#include "spell_none.hpp"
void SpellNone::trigger() { }
bool SpellNone::canTrigger() const { return false; }
void SpellNone::update(float /*elapsed*/) { }
int SpellNone::getExperienceCost() const { return 0; }
void SpellNone::onEquip() { }
void SpellNone::onUnEquip() { }
std::string SpellNone::getName() const { return "None"; }
