#ifndef GUARD_JAMTEMPLATE_CHARACTER_SHEET_IMGUI_HPP
#define GUARD_JAMTEMPLATE_CHARACTER_SHEET_IMGUI_HPP

#include "game_object.hpp"
#include "hud/observer_interface.hpp"
#include <map>
#include <memory>

class CharacterSheetImgui : public jt::GameObject {
public:
    CharacterSheetImgui(std::shared_ptr<ObserverInterface<int>> experienceObserver,
        std::shared_ptr<ObserverInterface<float>> healthObserver);
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;

    mutable bool m_drawCharacterSheet { false };

    float getHitpoints() const;
    float getHitpointsMax() const;
    void changeHitpoints(float delta);

    int getExperiencePoints() const;
    void changeExperiencePoints(int delta);

    float getMovementSpeedFactor() const;
    float getAttackSpeedFactor() const;
    float getDashFactor() const;

    void setMovementSpeedFactor(std::string const& identifier, float value);
    void setAttackSpeedFactor(std::string const& identifier, float value);
    void setDashFactor(std::string const& identifier, float value);

private:
    std::shared_ptr<ObserverInterface<int>> m_experienceObserver { nullptr };
    std::shared_ptr<ObserverInterface<float>> m_healthObserver { nullptr };

    std::map<std::string, float> m_movementSpeedFactorsAdditive;
    std::map<std::string, float> m_attackSpeedFactorsAdditive;
    std::map<std::string, float> m_dashFactorsAdditive;

    float m_hitpoints { 100.0f };
    float m_hitpointsMax { 100.0f };

    int m_experiencePoints { 0 };
};

#endif // GUARD_JAMTEMPLATE_CHARACTER_SHEET_IMGUI_HPP
