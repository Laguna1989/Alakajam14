#ifndef GUARD_JAMTEMPLATE_CHARACTER_SHEET_IMGUI_HPP
#define GUARD_JAMTEMPLATE_CHARACTER_SHEET_IMGUI_HPP

#include "game_object.hpp"
#include "hud/observer_interface.hpp"
#include "particle_system.hpp"
#include <functional>
#include <map>
#include <memory>

struct CharSheetObservers {
    std::shared_ptr<ObserverInterface<int>> experienceObserver;
    std::shared_ptr<ObserverInterface<float>> healthObserver;
    std::shared_ptr<ObserverInterface<float>> healthMaxObserver;
    std::function<void(void)> healCallback;
};

class CharacterSheetImgui : public jt::GameObject {
public:
    CharacterSheetImgui(CharSheetObservers observers);

    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;

    mutable bool m_drawCharacterSheet { false };

    float getHitpoints() const;
    float getHitpointsMax() const;
    void changeHitpoints(float damageValue) const;
    void changeHitpointsMax(float delta) const;

    int getExperiencePoints() const;
    void changeExperiencePoints(int delta) const;

    float getMovementSpeedFactor() const;
    float getAttackSpeedFactor() const;
    float getAttackDamageValue() const;
    float getDashFactor() const;
    float getArmorReductionValue() const;
    int getExpBoostValue() const;
    float getMagicDamageFactor() const;

    void setMovementSpeedFactor(std::string const& identifier, float value);
    void setAttackSpeedFactor(std::string const& identifier, float value);
    void setAttackDamageFactor(std::string const& identifier, float value);
    void setDashFactor(std::string const& identifier, float value);
    void setArmorReductionValue(std::string const& identifier, float value);
    void setExpBoostValue(std::string const&, int value);
    void setMagicDamageFactor(std::string const&, float value);

private:
    CharSheetObservers m_observers;

    std::map<std::string, float> m_movementSpeedFactorsAdditive;
    std::map<std::string, float> m_attackSpeedFactorsAdditive;
    std::map<std::string, float> m_attackDamageValueAdditive;
    std::map<std::string, float> m_dashFactorsAdditive;
    std::map<std::string, float> m_armorReductionValueAdditive;
    std::map<std::string, int> m_ExperienceBoostAdditive;
    std::map<std::string, float> m_magicDamageFactorsAdditive;

    mutable float m_baseMovementSpeed { 1.0f };
    mutable float m_baseDashSpeed { 1.0f };
    mutable float m_baseAttackSpeed { 1.0f };
    mutable float m_baseAttackDamage { 50.0f };
    mutable int m_baseExperienceBoost { 0 };

    mutable float m_armorReductionValue { 0.0f };
    mutable float m_baseMagicDamageFactor { 1.0f };

    mutable float m_hitpoints { 100.0f };
    mutable float m_hitpointsMax { 100.0f };

    mutable int m_experiencePoints { 0 };
    mutable int m_numberOfLevelUps { 1 };
    int getNextLevelUpCost() const;
};

#endif // GUARD_JAMTEMPLATE_CHARACTER_SHEET_IMGUI_HPP
