#include "spell_book.hpp"
#include "spell_attack_snipe.hpp"
#include "spell_none.hpp"
#include "spell_passive_attack_speed.hpp"
#include "spell_passive_dash.hpp"
#include "spell_passive_movement_speed.hpp"
#include "state_game.hpp"

SpellBook::SpellBook(StateGame& state)
{
    m_spells.push_back(std::make_shared<SpellNone>());
    m_spells.push_back(
        std::make_shared<SpellPassiveMovementSpeed>(*state.getPlayer()->getCharSheet()));
    m_spells.push_back(
        std::make_shared<SpellPassiveAttackSpeed>(*state.getPlayer()->getCharSheet()));
    m_spells.push_back(std::make_shared<SpellPassiveDash>(*state.getPlayer()->getCharSheet()));
    m_spells.push_back(std::make_shared<SpellAttackSnipe>(state));
}

std::shared_ptr<SpellInterface> SpellBook::getSpellByName(std::string const& name) const
{
    for (auto const& spell : m_spells) {
        if (spell->getName() == name) {
            return spell;
        }
    }
    return nullptr;
}

std::vector<std::string> SpellBook::getAllSpellNames() const
{
    std::vector<std::string> allNames {};
    allNames.resize(m_spells.size());

    std::transform(m_spells.begin(), m_spells.end(), allNames.begin(),
        [](auto const& spell) { return spell->getName(); });

    return allNames;
}

std::vector<std::string> SpellBook::getAvailableSpellNames() const { return m_availableSpells; }

void SpellBook::makeSpellAvailable(std::string const& name)
{
    auto sp = getSpellByName(name);
    if (sp == nullptr) {
        return;
    }

    m_availableSpells.push_back(name);
}
