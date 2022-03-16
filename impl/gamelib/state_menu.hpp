#ifndef GAME_STATE_MENU_HPP_INCLUDEGUARD
#define GAME_STATE_MENU_HPP_INCLUDEGUARD

#include "audio/sound.hpp"
#include "game_state.hpp"

namespace jt {

class Text;
class Shape;
class Animation;
class Sprite;

} // namespace jt

class StateMenu : public jt::GameState {
public:
    std::string getName() const override;

    void setScore(float s);

private:
    std::shared_ptr<jt::Shape> m_background;

    std::shared_ptr<jt::Animation> m_title;
    std::shared_ptr<jt::Animation> m_logo;

    std::shared_ptr<jt::Text> m_text_Explanation;
    std::shared_ptr<jt::Text> m_text_Credits;
    std::shared_ptr<jt::Text> m_text_Time;

    std::shared_ptr<jt::Shape> m_overlay;
    std::shared_ptr<jt::Sprite> m_vignette;

    std::shared_ptr<jt::Sound> m_startSound;
    std::shared_ptr<jt::Sound> m_menuMusic;

    bool m_started { false };

    void doInternalCreate() override;

    void createVignette();
    void createShapes();

    void createMenuText();
    void createTextCredits();
    void createTextExplanation();
    void createTextTitle();

    void createTweens();
    void createInstructionTweenScaleUp();
    void createInstructionTweenScaleDown();
    void createTweenOverlayAlpha();
    void createTweenCreditsPosition();
    void createTweenExplanationScale();

    void doInternalUpdate(float const elapsed) override;
    void updateDrawables(const float& elapsed);
    void checkForTransitionToStateGame();
    void startTransitionToStateGame();

    void doInternalDraw() const override;
    float m_time { 0.0f };
};

#endif
