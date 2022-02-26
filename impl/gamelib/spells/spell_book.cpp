#include "spell_book.hpp"
#include "game_interface.hpp"
#include "imgui.h"
#include "spell_attack_snipe.hpp"
#include "spell_none.hpp"
#include "spell_passive_attack_speed.hpp"
#include "spell_passive_dash.hpp"
#include "spell_passive_movement_speed.hpp"
#include "state_game.hpp"
#include "strutils.hpp"

SpellBook::SpellBook(StateGame& state)
{
    m_spells.push_back(std::make_shared<SpellNone>());
    m_spells.push_back(
        std::make_shared<SpellPassiveMovementSpeed>(*state.getPlayer()->getCharSheet()));
    m_spells.push_back(
        std::make_shared<SpellPassiveAttackSpeed>(*state.getPlayer()->getCharSheet()));
    m_spells.push_back(std::make_shared<SpellPassiveDash>(*state.getPlayer()->getCharSheet()));
    m_spells.push_back(std::make_shared<SpellAttackSnipe>(state));

    m_equippedSpells.push_back(getSpellByName("Snipe"));
    m_equippedSpells.push_back(getSpellByName("None"));
    m_equippedSpells.push_back(getSpellByName("None"));
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

void SpellBook::doUpdate(float const)
{
    if (getGame()->input().keyboard()->justPressed(jt::KeyCode::L)) {
        m_drawSpellbook = !m_drawSpellbook;
    }
}

void SpellBook::doDraw() const
{
    if (!m_drawSpellbook) {
        return;
    }
    ImGui::SetNextWindowPos(ImVec2 { 400, 0 });

    ImGui::SetNextWindowSize(ImVec2 { 400, 600 });
    ImGui::Begin("Spellbook", &m_drawSpellbook);

    drawEquippedSpells();
    ImGui::End();
}

void SpellBook::drawEquippedSpells() const
{
    ImGui::Text("Equipped Spells");

    for (auto i = 0U; i != m_equippedSpells.size(); ++i) {
        std::string slotName = std::to_string(i);
        ImGui::Text("Spell %s", slotName.c_str());
        ImGui::SameLine();

        std::string displayName = m_equippedSpells.at(i)->getName() + "##" + slotName;
        if (ImGui::Button(displayName.c_str())) {
            ImGui::OpenPopup(slotName.c_str());
        }

        if (ImGui::BeginPopup(slotName.c_str())) {
            if (strutil::starts_with(displayName, "None")) {
                ImGui::Text("Nothing Equipped");
            }

            auto const possibleSpells = getEquippableSpells();
            for (auto const& spell : possibleSpells) {
                if (ImGui::Selectable(spell.c_str())) {
                    m_equippedSpells.at(i)->onUnEquip();
                    m_equippedSpells.at(i) = getSpellByName(spell);
                    m_equippedSpells.at(i)->onEquip();
                }
            }

            ImGui::EndPopup();
        }
    }
}

std::vector<std::shared_ptr<SpellInterface>> SpellBook::getEquippedSpells()
{
    return m_equippedSpells;
}

std::vector<std::string> SpellBook::getEquippableSpells() const
{
    std::vector<std::string> values;

    for (auto const& currentSpellName : m_availableSpells) {
        if (std::find_if(m_equippedSpells.cbegin(), m_equippedSpells.cend(),
                [&currentSpellName](auto const& potentialSpellToBeAdded) {
                    return potentialSpellToBeAdded->getName() == currentSpellName;
                })
            == m_equippedSpells.end()) {
            values.push_back(currentSpellName);
        }
    }

    return values;
}
