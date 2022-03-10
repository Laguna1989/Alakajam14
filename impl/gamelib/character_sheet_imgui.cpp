#include "character_sheet_imgui.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "imgui.h"
#include "math_helper.hpp"

CharacterSheetImgui::CharacterSheetImgui(CharSheetObservers observers)
    : m_observers { observers }
{
}

void CharacterSheetImgui::doCreate() { m_observers.healthObserver->notify(m_hitpoints); }

void CharacterSheetImgui::doUpdate(float const)
{
    if (getGame()->input().keyboard()->justPressed(jt::KeyCode::C)) {
        m_drawCharacterSheet = !m_drawCharacterSheet;
    }
}

int CharacterSheetImgui::getNextLevelUpCost() const { return 8 + m_numberOfLevelUps * 5; }

void CharacterSheetImgui::doDraw() const
{
    if (!m_drawCharacterSheet) {
        return;
    }
    ImGui::SetNextWindowPos(ImVec2 { 0, 0 });
    ImGui::SetNextWindowSize(ImVec2 { 400, 600 });
    ImGui::Begin("Fungus McShroom", &m_drawCharacterSheet);

    ImGui::Text("Experience: %s", std::to_string(m_experiencePoints).c_str());
    ImGui::Text("Health: %s / %s", jt::MathHelper::floatToStringWithXDigits(m_hitpoints, 1).c_str(),
        jt::MathHelper::floatToStringWithXDigits(m_hitpointsMax, 1).c_str());
    ImGui::SameLine();

    std::string const increaseHPText = "+"
        + jt::MathHelper::floatToStringWithXDigits(GP::LevelUpHealthIncrease(), 1)
        + "HP (cost: " + std::to_string(getNextLevelUpCost()) + " XP)##hitpoints";

    if (ImGui::Button(increaseHPText.c_str())) {
        if (m_experiencePoints >= getNextLevelUpCost()) {
            float ratio = m_hitpoints / m_hitpointsMax;

            changeExperiencePoints(-getNextLevelUpCost());
            changeHitpointsMax(GP::LevelUpHealthIncrease());
            changeHitpoints(-GP::LevelUpHealthIncrease());
            m_numberOfLevelUps++;
        }
    }

    ImGui::Separator();

    ImGui::Text("Movement speed factor: %s",
        jt::MathHelper::floatToStringWithXDigits(getMovementSpeedFactor(), 3).c_str());
    ImGui::SameLine();
    {
        std::string const increaseSpeedText = "+"
            + jt::MathHelper::floatToStringWithXDigits(
                GP::LevelUpMovementSpeedIncrease() * 100.0f, 1)
            + "% (cost: " + std::to_string(getNextLevelUpCost()) + " XP)##speed";
        if (ImGui::Button(increaseSpeedText.c_str())) {
            if (m_experiencePoints >= getNextLevelUpCost()) {
                changeExperiencePoints(-getNextLevelUpCost());
                m_baseMovementSpeed += GP::LevelUpMovementSpeedIncrease();
                m_numberOfLevelUps++;
            }
        }
    }

    ImGui::Text("Dash speed factor: %s",
        jt::MathHelper::floatToStringWithXDigits(getDashFactor(), 2).c_str());
    ImGui::SameLine();
    {
        std::string const increaseDashText = "+"
            + jt::MathHelper::floatToStringWithXDigits(
                GP::LevelUpMovementSpeedIncrease() * 100.0f, 2)
            + "% (cost: " + std::to_string(getNextLevelUpCost()) + " XP)##dash";
        if (ImGui::Button(increaseDashText.c_str())) {
            if (m_experiencePoints >= getNextLevelUpCost()) {
                changeExperiencePoints(-getNextLevelUpCost());
                m_baseDashSpeed += 0.015f;
                m_numberOfLevelUps++;
            }
        }
    }

    ImGui::Text("Attack speed factor: %s",
        jt::MathHelper::floatToStringWithXDigits(getAttackSpeedFactor(), 2).c_str());
    ImGui::SameLine();
    {
        std::string const increaseAttackSpeedText
            = "+2.5% (cost: " + std::to_string(getNextLevelUpCost()) + " XP)##attack";
        if (ImGui::Button(increaseAttackSpeedText.c_str())) {
            if (m_experiencePoints >= getNextLevelUpCost()) {
                changeExperiencePoints(-getNextLevelUpCost());
                m_baseAttackSpeed += 0.025f;
                m_numberOfLevelUps++;
            }
        }
    }

    ImGui::Text("Attack damage: %s",
        jt::MathHelper::floatToStringWithXDigits(getAttackDamageValue(), 2).c_str());
    ImGui::SameLine();
    {
        std::string const increaseAttackDamageText = "+"
            + jt::MathHelper::floatToStringWithXDigits(GP::LevelUpAttackDamageIncrease(), 2)
            + " (cost: " + std::to_string(getNextLevelUpCost()) + " XP)";
        if (ImGui::Button(increaseAttackDamageText.c_str())) {
            if (m_experiencePoints >= getNextLevelUpCost()) {
                changeExperiencePoints(-getNextLevelUpCost());
                m_baseAttackDamage += GP::LevelUpAttackDamageIncrease();
                m_numberOfLevelUps++;
            }
        }
    }

    ImGui::Text("Spell damage factor: %s",
        jt::MathHelper::floatToStringWithXDigits(getMagicDamageFactor(), 2).c_str());
    ImGui::SameLine();
    {
        std::string const increaseMagicDamageText = "+"
            + jt::MathHelper::floatToStringWithXDigits(GP::LevelUpMagicDamageIncrease(), 2)
            + "% (cost: " + std::to_string(getNextLevelUpCost()) + " XP)";
        if (ImGui::Button(increaseMagicDamageText.c_str())) {
            if (m_experiencePoints >= getNextLevelUpCost()) {
                changeExperiencePoints(-getNextLevelUpCost());
                m_baseMagicDamageFactor += GP::LevelUpMagicDamageIncrease() / 100.0f;
                m_numberOfLevelUps++;
            }
        }
    }

    ImGui::Separator();
    ImGui::Text("Armor Damage reduction %s",
        jt::MathHelper::floatToStringWithXDigits(getArmorReductionValue(), 2).c_str());

    ImGui::End();
}

float CharacterSheetImgui::getHitpoints() const { return m_hitpoints; }
float CharacterSheetImgui::getHitpointsMax() const { return m_hitpointsMax; }

void CharacterSheetImgui::changeHitpoints(float damageValue) const
{
    if (damageValue > 0) {
        damageValue -= getArmorReductionValue();
        if (damageValue < 0.0f) {
            damageValue = 0.0f;
        }
    }
    m_hitpoints -= damageValue;
    m_observers.healthObserver->notify(m_hitpoints);
    if (damageValue <= 0) {
        if (m_observers.healCallback) {
            m_observers.healCallback();
        }
    }
}

void CharacterSheetImgui::changeHitpointsMax(float delta) const
{
    // TODO visual candy
    m_hitpointsMax += delta;
    m_observers.healthMaxObserver->notify(m_hitpointsMax);
}

int CharacterSheetImgui::getExperiencePoints() const { return m_experiencePoints; }
void CharacterSheetImgui::changeExperiencePoints(int delta) const
{
    m_experiencePoints += delta + getExpBoostValue();
    m_observers.experienceObserver->notify(m_experiencePoints);
}

float CharacterSheetImgui::getMovementSpeedFactor() const
{
    float v = 0.0f;

    for (auto const& kvp : m_movementSpeedFactorsAdditive) {
        v += kvp.second;
    }

    return m_baseMovementSpeed + v;
}

float CharacterSheetImgui::getAttackSpeedFactor() const
{
    float v = 0.0f;

    for (auto const& kvp : m_attackSpeedFactorsAdditive) {
        v += kvp.second;
    }

    return m_baseAttackSpeed + v;
}

float CharacterSheetImgui::getDashFactor() const
{
    float v = 0.0f;

    for (auto const& kvp : m_dashFactorsAdditive) {
        v += kvp.second;
    }

    return m_baseDashSpeed + v;
}

float CharacterSheetImgui::getAttackDamageValue() const
{
    float v = 0.0f;

    for (auto const& kvp : m_attackDamageValueAdditive) {
        v += kvp.second;
    }
    return m_baseAttackDamage + v;
}

void CharacterSheetImgui::setMovementSpeedFactor(std::string const& identifier, float value)
{
    m_movementSpeedFactorsAdditive[identifier] = value;
}

void CharacterSheetImgui::setAttackSpeedFactor(std::string const& identifier, float value)
{
    m_attackSpeedFactorsAdditive[identifier] = value;
}
void CharacterSheetImgui::setAttackDamageFactor(std::string const& identifier, float value)
{
    m_attackDamageValueAdditive[identifier] = value;
}
void CharacterSheetImgui::setDashFactor(std::string const& identifier, float value)
{
    m_dashFactorsAdditive[identifier] = value;
}

void CharacterSheetImgui::setArmorReductionValue(std::string const& identifier, float value)
{
    m_armorReductionValueAdditive[identifier] = value;
}

void CharacterSheetImgui::setExpBoostValue(std::string const& identifier, int value)
{
    m_ExperienceBoostAdditive[identifier] = value;
}

void CharacterSheetImgui::setMagicDamageFactor(std::string const& identifier, float value)
{
    m_magicDamageFactorsAdditive[identifier] = value;
}

float CharacterSheetImgui::getArmorReductionValue() const
{
    float v = 0.0f;

    for (auto const& kvp : m_armorReductionValueAdditive) {
        v += kvp.second;
    }
    return m_armorReductionValue + v;
}

int CharacterSheetImgui::getExpBoostValue() const
{
    int v = 0;

    for (auto const& kvp : m_ExperienceBoostAdditive) {
        v += kvp.second;
    }
    return m_baseExperienceBoost + v;
}
float CharacterSheetImgui::getMagicDamageFactor() const
{
    float v = 0.0f;

    for (auto const& kvp : m_magicDamageFactorsAdditive) {
        v += kvp.second;
    }
    return m_baseMagicDamageFactor + v;
}
