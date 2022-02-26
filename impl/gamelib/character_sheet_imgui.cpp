#include "character_sheet_imgui.hpp"
#include "game_interface.hpp"
#include "imgui.h"
#include "math_helper.hpp"

CharacterSheetImgui::CharacterSheetImgui(std::shared_ptr<ObserverInterface<int>> observer)
    : m_experienceObserver { observer }
{
}

void CharacterSheetImgui::doUpdate(float const)
{
    if (getGame()->input().keyboard()->justPressed(jt::KeyCode::C)) {
        m_drawCharacterSheet = !m_drawCharacterSheet;
    }
}
void CharacterSheetImgui::doDraw() const
{
    if (!m_drawCharacterSheet) {
        return;
    }
    ImGui::SetNextWindowPos(ImVec2 { 0, 0 });

    ImGui::SetNextWindowSize(ImVec2 { 400, 600 });
    ImGui::Begin("PlayerCharacter", &m_drawCharacterSheet);

    ImGui::Text("Movement speed factor: %s",
        jt::MathHelper::floatToStringWithXDigits(getMovementSpeedFactor(), 2).c_str());
    ImGui::Text("Dash speed factor: %s",
        jt::MathHelper::floatToStringWithXDigits(getDashFactor(), 2).c_str());

    ImGui::Text("Attackda speed factor: %s",
        jt::MathHelper::floatToStringWithXDigits(getAttackSpeedFactor(), 2).c_str());

    ImGui::End();
}

float CharacterSheetImgui::getHitpoints() const { return m_hitpoints; }
float CharacterSheetImgui::getHitpointsMax() const { return m_hitpointsMax; }
void CharacterSheetImgui::changeHitpoints(float delta) { m_hitpoints -= delta; }

int CharacterSheetImgui::getExperiencePoints() const { return m_experiencePoints; }
void CharacterSheetImgui::changeExperiencePoints(int delta)
{
    m_experiencePoints += delta;
    m_experienceObserver->notify(m_experiencePoints);
}

float CharacterSheetImgui::getMovementSpeedFactor() const
{
    float v = 0.0f;

    for (auto const& kvp : m_movementSpeedFactorsAdditive) {
        v += kvp.second;
    }

    // TODO incorporate items
    return 1.0f + v;
}

float CharacterSheetImgui::getAttackSpeedFactor() const
{
    float v = 0.0f;

    for (auto const& kvp : m_attackSpeedFactorsAdditive) {
        v += kvp.second;
    }

    // TODO incorporate items
    return 1.0f + v;
}

float CharacterSheetImgui::getDashFactor() const
{
    float v = 0.0f;

    for (auto const& kvp : m_dashFactorsAdditive) {
        v += kvp.second;
    }

    // TODO incorporate items
    return 1.0f + v;
}

void CharacterSheetImgui::setMovementSpeedFactor(std::string const& identifier, float value)
{
    m_movementSpeedFactorsAdditive[identifier] = value;
}
void CharacterSheetImgui::setAttackSpeedFactor(std::string const& identifier, float value)
{
    m_attackSpeedFactorsAdditive[identifier] = value;
}
void CharacterSheetImgui::setDashFactor(std::string const& identifier, float value)
{
    m_dashFactorsAdditive[identifier] = value;
}
