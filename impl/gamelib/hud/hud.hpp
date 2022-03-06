#ifndef GAME_HUD_HPP_INCLUDEGUARD
#define GAME_HUD_HPP_INCLUDEGUARD

#include "bar.hpp"
#include "game_object.hpp"
#include "health_display.hpp"
#include "observer_interface.hpp"
#include "sprite.hpp"
#include "text.hpp"

class ExperienceDisplay;

class Hud : public jt::GameObject {
public:
    std::shared_ptr<ObserverInterface<int>> getObserverExperience() const;
    std::shared_ptr<ObserverInterface<float>> getObserverHealth() const;
    std::shared_ptr<ObserverInterface<float>> getObserverHealthMax() const;

private:
    std::shared_ptr<ExperienceDisplay> m_displayExperience;
    std::shared_ptr<HealthDisplay> m_displayHealth;
    std::shared_ptr<HealthMaxDisplay> m_displayHealthMax;

    jt::Text::Sptr m_TextExperience;
    jt::Bar::Sptr m_healthBar;

    void doCreate() override;

    void doUpdate(float const elapsed) override;

    void doDraw() const override;
};

#endif
