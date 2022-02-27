#include "state_menu.hpp"
#include "color.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "input/input_manager.hpp"
#include "input/key_codes.hpp"
#include "lerp.hpp"
#include "logging/license_info.hpp"
#include "math_helper.hpp"
#include "state_game.hpp"
#include "state_manager/state_manager_transition_fade_to_black.hpp"
#include "text.hpp"
#include "tweens/tween_alpha.hpp"
#include "tweens/tween_position.hpp"
#include "tweens/tween_scale.hpp"
#include <algorithm>

void StateMenu::doInternalCreate()
{
    createMenuText();
    createShapes();
    createVignette();

    createTweens();

    add(std::make_shared<jt::LicenseInfo>());

    getGame()->getStateManager().setTransition(
        std::make_shared<jt::StateManagerTransitionFadeToBlack>(
            GP::GetScreenSize(), getGame()->gfx().textureManager()));

    m_startSound = std::make_shared<jt::Sound>("assets/sound/main_menu_press_space.ogg");
    m_startSound->setVolume(0.4f);

    m_menuMusic = std::make_shared<jt::Sound>("assets/sound/alaka2022_title_menu_v1_loop.ogg");
    m_menuMusic->setLoop(true);
    m_menuMusic->play();
}

void StateMenu::createVignette()
{
    m_vignette = jt::dh::createVignette(GP::GetScreenSize(), getGame()->gfx().textureManager());
    m_vignette->setColor({ 255, 255, 255, 110 });
}

void StateMenu::createShapes()
{
    m_background = jt::dh::createShapeRect(
        GP::GetScreenSize(), GP::getPalette().getColor(0), getGame()->gfx().textureManager());
    m_overlay = jt::dh::createShapeRect(
        GP::GetScreenSize(), jt::colors::Black, getGame()->gfx().textureManager());
}

void StateMenu::createMenuText()
{
    createTextTitle();
    createTextExplanation();
    createTextCredits();
}

void StateMenu::createTextCredits()
{
    m_text_Credits = jt::dh::createText(getGame()->gfx().target(),
        "Created by " + GP::AuthorName() + "for " + GP::JamName() + "\n" + GP::JamDate()
            + "\n\nF9 for License Information",
        10U, GP::getPalette().getColor(4));
    m_text_Credits->setTextAlign(jt::Text::TextAlign::LEFT);
    m_text_Credits->setPosition({ 10, GP::GetScreenSize().y - 100 });
    m_text_Credits->setShadow(GP::PaletteFontShadow(), jt::Vector2f { 1, 1 });
}

void StateMenu::createTextExplanation()
{
    float half_width = GP::GetScreenSize().x / 2;
    m_text_Explanation = jt::dh::createText(getGame()->gfx().target(),
        "Press Space to start the game", 16U, GP::getPalette().getColor(7));
    m_text_Explanation->setPosition({ half_width, 100 });
    m_text_Explanation->setShadow(GP::PaletteFontShadow(), jt::Vector2f { 3, 3 });
}

void StateMenu::createTextTitle()
{
    float half_width = GP::GetScreenSize().x / 2;

    m_title = std::make_shared<jt::Animation>();
    m_title->add("assets/menu.png", "idle", { 224u, 32u }, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        0.2f, getGame()->gfx().textureManager());
    m_title->setPosition({ half_width - 224.0f / 2.0f, 20 });
    m_title->play("idle");
}

void StateMenu::createTweens()
{
    createTweenOverlayAlpha();
    createTweenCreditsPosition();
    createTweenExplanationScale();
}

void StateMenu::createInstructionTweenScaleUp()
{
    auto ts = jt::TweenScale::create(
        m_text_Explanation, 0.75f, jt::Vector2f { 1.0f, 1.0f }, jt::Vector2f { 1.05f, 1.05f });
    ts->setAgePercentConversion([](float age) {
        return jt::Lerp::cosine(0.0f, 1.0f, jt::MathHelper::clamp(age, 0.0f, 1.0f));
    });
    ts->addCompleteCallback([this]() { createInstructionTweenScaleDown(); });
    add(ts);
}
void StateMenu::createInstructionTweenScaleDown()
{
    auto ts = jt::TweenScale::create(
        m_text_Explanation, 0.75f, jt::Vector2f { 1.05f, 1.05f }, jt::Vector2f { 1.0f, 1.0f });
    ts->setAgePercentConversion([](float age) {
        return jt::Lerp::cosine(0.0f, 1.0f, jt::MathHelper::clamp(age, 0.0f, 1.0f));
    });
    ts->addCompleteCallback([this]() { createInstructionTweenScaleUp(); });
    add(ts);
}

void StateMenu::createTweenExplanationScale()
{
    auto s2 = m_text_Explanation->getPosition() + jt::Vector2f { -1000, 0 };
    auto e2 = m_text_Explanation->getPosition();

    auto tween = jt::TweenPosition::create(m_text_Explanation, 0.5f, s2, e2);
    tween->setStartDelay(0.3f);
    tween->setSkipFrames();

    tween->addCompleteCallback([this]() { createInstructionTweenScaleUp(); });
    add(tween);
}

void StateMenu::createTweenOverlayAlpha()
{
    auto tween = jt::TweenAlpha::create(m_overlay, 0.5f, std::uint8_t { 255 }, std::uint8_t { 0 });
    tween->setSkipFrames();
    add(tween);
}

void StateMenu::createTweenCreditsPosition()
{
    auto s3 = m_text_Credits->getPosition() + jt::Vector2f { 0, 100 };
    auto e3 = m_text_Credits->getPosition();

    auto tween = jt::TweenPosition::create(m_text_Credits, 0.35f, s3, e3);
    tween->setStartDelay(0.8f);
    tween->setSkipFrames();
    add(tween);
}

void StateMenu::doInternalUpdate(float const elapsed)
{
    m_startSound->update();
    m_menuMusic->update();
    updateDrawables(elapsed);
    checkForTransitionToStateGame();

    if (m_started) {
        // cheapo fade-out
        m_menuMusic->setVolume(m_menuMusic->getVolume() * 0.95f);
    }
}

void StateMenu::updateDrawables(const float& elapsed)
{
    m_background->update(elapsed);
    m_title->update(elapsed);
    m_text_Explanation->update(elapsed);
    m_text_Credits->update(elapsed);
    m_overlay->update(elapsed);
    m_vignette->update(elapsed);
}

void StateMenu::checkForTransitionToStateGame()
{
    auto const keysToTriggerTransition = { jt::KeyCode::Space, jt::KeyCode::Enter };

    if (std::any_of(keysToTriggerTransition.begin(), keysToTriggerTransition.end(),
            [this](auto const k) { return getGame()->input().keyboard()->justPressed(k); })) {
        startTransitionToStateGame();
    }
}

void StateMenu::startTransitionToStateGame()
{
    if (!m_started) {
        m_started = true;

        m_startSound->play();

        getGame()->getStateManager().switchState(std::make_shared<StateGame>());
    }
}

void StateMenu::doInternalDraw() const
{
    m_background->draw(getGame()->gfx().target());

    m_title->draw(getGame()->gfx().target());
    m_text_Explanation->draw(getGame()->gfx().target());
    m_text_Credits->draw(getGame()->gfx().target());

    m_overlay->draw(getGame()->gfx().target());
    m_vignette->draw(getGame()->gfx().target());
}

std::string StateMenu::getName() const { return "Menu"; }
