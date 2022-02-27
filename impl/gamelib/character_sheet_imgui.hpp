#ifndef GUARD_JAMTEMPLATE_CHARACTER_SHEET_IMGUI_HPP
#define GUARD_JAMTEMPLATE_CHARACTER_SHEET_IMGUI_HPP

#include "game_object.hpp"
#include "hud/observer_interface.hpp"
#include <map>
#include <memory>

class CharacterSheetImgui : public jt::GameObject {
public:
    CharacterSheetImgui(std::shared_ptr<ObserverInterface<int>> experienceObserver,
        std::shared_ptr<ObserverInterface<float>> healthObserver,
        std::shared_ptr<ObserverInterface<float>> healthMaxObserver);
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;

    mutable bool m_drawCharacterSheet { false };

    float getHitpoints() const;
    float getHitpointsMax() const;
    void changeHitpoints(float delta) const;
    void changeHitpointsMax(float delta) const;

    int getExperiencePoints() const;
    void changeExperiencePoints(int delta) const;

    float getMovementSpeedFactor() const;
    float getAttackSpeedFactor() const;
    float getAttackDamageValue() const;
    float getDashFactor() const;

    void setMovementSpeedFactor(std::string const& identifier, float value);
    void setAttackSpeedFactor(std::string const& identifier, float value);
    void setDashFactor(std::string const& identifier, float value);

private:
    std::shared_ptr<ObserverInterface<int>> m_experienceObserver { nullptr };
    std::shared_ptr<ObserverInterface<float>> m_healthObserver { nullptr };
    std::shared_ptr<ObserverInterface<float>> m_healthMaxObserver { nullptr };

    std::map<std::string, float> m_movementSpeedFactorsAdditive;
    std::map<std::string, float> m_attackSpeedFactorsAdditive;
    std::map<std::string, float> m_dashFactorsAdditive;

    mutable float m_baseMovementSpeed { 1.0f };
    mutable float m_baseDashSpeed { 1.0f };
    mutable float m_baseAttackSpeed { 1.0f };
    mutable float m_baseAttackDamage { 50.0f };

    mutable float m_hitpoints { 100.0f };
    mutable float m_hitpointsMax { 100.0f };

    mutable int m_experiencePoints { 0 };
    mutable int m_numberOfLevelUps { 1 };
    int getNextLevelUpCost() const;
};

#endif // GUARD_JAMTEMPLATE_CHARACTER_SHEET_IMGUI_HPP
