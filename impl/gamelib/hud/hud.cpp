#include "hud.hpp"
#include "../game_properties.hpp"
#include "color.hpp"
#include "game_interface.hpp"
#include "score_display.hpp"

std::shared_ptr<ObserverInterface<int>> Hud::getObserverExperience() const
{
    return m_displayExperience;
}
std::shared_ptr<ObserverInterface<float>> Hud::getObserverHealth() const { return m_displayHealth; }

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

    m_healthBar = std::make_shared<jt::Bar>(96.0f, 16.0f, true, getGame()->gfx().textureManager());
    m_healthBar->setIgnoreCamMovement(true);
    m_healthBar->setPosition(jt::Vector2f { GP::GetScreenSize().x - 96.0f - 8.0f, 8.0f });
    m_healthBar->setMaxValue(100.0f);
    m_healthBar->setFrontColor(jt::Color { 200, 0, 0 });
    m_healthBar->setBackColor(jt::Color { 20, 20, 20 });
    m_displayHealth = std::make_shared<HealthDisplay>(m_healthBar);
}

void Hud::doUpdate(float const elapsed)
{
    m_TextExperience->update(elapsed);
    m_healthBar->update(elapsed);
}

void Hud::doDraw() const
{
    m_TextExperience->draw(getGame()->gfx().target());
    m_healthBar->draw(getGame()->gfx().target());
}
