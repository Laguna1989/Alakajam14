#include "spell_book.hpp"
#include "game_interface.hpp"
#include "imgui.h"
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

    m_equippedSpells.push_back(getSpellByName("None"));
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
        ImGui::Text("Spell %i", i);
        ImGui::SameLine();

        std::string displayName = m_equippedSpells.at(i)->getName();
        if (ImGui::Button(displayName.c_str())) {
            ImGui::OpenPopup(displayName.c_str());
        }

        if (ImGui::BeginPopup(displayName.c_str())) {
            if (displayName == "None") {
                ImGui::Text("Nothing Equipped");
            } else {
                std::string unequipButtonText = "Unequip##" + displayName;
                // TODO unequip spell
                //                if (ImGui::Selectable(unequipButtonText.c_str())) {
                //                    m_itemToUnequip = kvp.second;
                //                }
            }

            //            auto const possibleItems = getItemReferenceIdsForEquipmentSlot(kvp.first);
            //            for (auto const& itemReferenceId : possibleItems) {
            //                if (kvp.second == itemReferenceId) {
            //                    continue;
            //                }
            //                auto itemReference
            //                    =
            //                    m_repository.lock()->getItemReferenceFromString(itemReferenceId);
            //                std::string const itemName = itemReference->listName;
            //                if (ImGui::Selectable(itemName.c_str())) {
            //                    if (!kvp.second.empty()) {
            //                        m_itemToUnequip = kvp.second;
            //                    }
            //                    m_itemToEquip = itemReferenceId;
            //                }
            //            }

            ImGui::EndPopup();
        }
    }
}
std::vector<std::shared_ptr<SpellInterface>> SpellBook::getEquippedSpells()
{
    return m_equippedSpells;
}
