#include "hud.hpp"
#include "../game_properties.hpp"
#include "color.hpp"
#include "game_interface.hpp"
#include "score_display.hpp"

std::shared_ptr<ObserverInterface<int>> Hud::getObserverExperience() const
{
    return m_displayExperience;
}

void Hud::doCreate()
{
    m_TextExperience = std::make_shared<jt::Text>();
    m_TextExperience->loadFont("assets/font.ttf", 16, getGame()->gfx().target());
    m_TextExperience->setColor(jt::Color { 248, 249, 254 });
    m_TextExperience->update(0.0f);
    m_TextExperience->setTextAlign(jt::Text::TextAlign::LEFT);
    m_TextExperience->setPosition({ 10, 4 });
    m_TextExperience->setIgnoreCamMovement(true);

    m_displayExperience = std::make_shared<ScoreDisplay>(m_TextExperience, "Experience: ");
}

void Hud::doUpdate(float const elapsed) { m_TextExperience->update(elapsed); }

void Hud::doDraw() const { m_TextExperience->draw(getGame()->gfx().target()); }
