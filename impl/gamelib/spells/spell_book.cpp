#include "spell_book.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "imgui.h"
#include "spell_attack_broad.hpp"
#include "spell_attack_snipe.hpp"
#include "spell_heal_large.hpp"
#include "spell_heal_small.hpp"
#include "spell_none.hpp"
#include "spell_passive_armor.hpp"
#include "spell_passive_attack_damage.hpp"
#include "spell_passive_attack_speed.hpp"
#include "spell_passive_dash.hpp"
#include "spell_passive_exp_boost.hpp"
#include "spell_passive_magic_damage.hpp"
#include "spell_passive_movement_speed.hpp"
#include "state_game.hpp"
#include "strutils.hpp"

namespace {
std::string getKeysForId(std::size_t id)
{
    if (id == 0) {
        return "[Q, 1]";
    } else if (id == 1) {
        return "[E, 2]";
    } else {
        return "[Tab, 3]";
    }
}
} // namespace

SpellBook::SpellBook(StateGame& state)
{
    m_spells.push_back(std::make_shared<SpellNone>());
    m_spells.push_back(
        std::make_shared<SpellPassiveMovementSpeed>(*state.getPlayer()->getCharSheet()));
    m_spells.push_back(
        std::make_shared<SpellPassiveAttackSpeed>(*state.getPlayer()->getCharSheet()));
    m_spells.push_back(std::make_shared<SpellPassiveDash>(*state.getPlayer()->getCharSheet()));
    m_spells.push_back(std::make_shared<SpellPassiveArmor>(*state.getPlayer()->getCharSheet()));

    m_spells.push_back(std::make_shared<SpellAttackSnipe>(state));
    m_spells.push_back(std::make_shared<SpellAttackBroad>(state));
    m_spells.push_back(std::make_shared<SpellHealSmall>(*state.getPlayer()->getCharSheet()));
    m_spells.push_back(std::make_shared<SpellHealLarge>(*state.getPlayer()->getCharSheet()));

    m_spells.push_back(
        std::make_shared<SpellPassiveAttackDamage>(*state.getPlayer()->getCharSheet()));
    m_spells.push_back(std::make_shared<SpellPassiveExpBoost>(*state.getPlayer()->getCharSheet()));
    m_spells.push_back(
        std::make_shared<SpellPassiveMagicDamage>(*state.getPlayer()->getCharSheet()));

    m_equippedSpells.push_back(getSpellByName("None"));
    m_equippedSpells.push_back(getSpellByName("None"));
    m_equippedSpells.push_back(getSpellByName("None"));
}

void SpellBook::doCreate()
{
    m_selectSound = std::make_shared<jt::SoundNull>("assets/sound/spellbook_click.ogg");
    m_selectSound->setVolume(0.5f);
    getGame()->audio().addTemporarySound(m_selectSound);

    m_newSpellText = jt::dh::createText(getGame()->gfx().target(), "New Spells!", 12);
    m_newSpellText->setIgnoreCamMovement(true);
    m_newSpellText->setPosition(jt::Vector2f { 350, 280 });

    for (auto i = 0U; i != m_equippedSpells.size(); ++i) {
        auto text = jt::dh::createText(getGame()->gfx().target(), getKeysForId(i) + " None", 12);
        text->setIgnoreCamMovement(true);
        text->setPosition(jt::Vector2f { 10.0f, 240.0f + i * 20.0f });
        text->setTextAlign(jt::Text::TextAlign::LEFT);
        m_spellTexts.push_back(text);
    }
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
    if (std::count(m_availableSpells.begin(), m_availableSpells.end(), name) != 0) {
        return;
    }
    auto sp = getSpellByName(name);
    if (sp == nullptr) {
        return;
    }

    m_availableSpells.push_back(name);
    m_newSpell = true;
}

void SpellBook::doUpdate(float const elapsed)
{
    float v = sin(getAge() * 2.5f);
    auto a = 100 + 155 * v * v;
    m_newSpellText->setColor(jt::Color { 255, 255, 255, static_cast<std::uint8_t>(a) });
    m_newSpellText->update(elapsed);

    if (getGame()->input().keyboard()->justPressed(jt::KeyCode::L)) {
        m_drawSpellbook = !m_drawSpellbook;
    }

    for (auto const& t : m_spellTexts) {
        t->update(elapsed);
    }
}

void SpellBook::doDraw() const
{
    if (m_newSpell) {
        m_newSpellText->draw(getGame()->gfx().target());
    }
    for (auto const& t : m_spellTexts) {
        t->draw(getGame()->gfx().target());
    }

    if (!m_drawSpellbook) {
        return;
    }
    m_newSpell = false;
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
            m_selectSound->play();

            ImGui::OpenPopup(slotName.c_str());
        }

        if (ImGui::BeginPopup(slotName.c_str())) {

            auto const possibleSpells = getEquippableSpells();
            if (possibleSpells.empty()) {
                ImGui::Text("Nothing available");
            } else {
                for (auto const& spell : possibleSpells) {
                    if (ImGui::Selectable(spell.c_str())) {
                        m_equippedSpells.at(i)->onUnEquip();
                        m_equippedSpells.at(i) = getSpellByName(spell);
                        m_equippedSpells.at(i)->onEquip();
                        auto const cost = getSpellByName(spell)->getExperienceCost();
                        auto const xpString
                            = (cost == 0 ? "" : " - " + std::to_string(cost) + "XP");
                        m_spellTexts.at(i)->setText(getKeysForId(i) + " " + spell + xpString);
                    }
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
        values.push_back(currentSpellName);
    }

    return values;
}
std::vector<std::shared_ptr<jt::Text>> SpellBook::getEquippedSpellTexts() { return m_spellTexts; }
