#ifndef GUARD_JAMTEMPLATE_CHARACTER_HPP
#define GUARD_JAMTEMPLATE_CHARACTER_HPP

#include "animation.hpp"
#include "audio/sound.hpp"
#include "audio/sound_group.hpp"
#include "box2dwrapper/box2d_object.hpp"
#include "character_sheet_imgui.hpp"
#include "damage.hpp"
#include "game_object.hpp"
#include "input/key_codes.hpp"
#include "spells/spell_attack_snipe.hpp"
#include "spells/spell_book.hpp"
#include <memory>
#include <string>

class StateGame;
class PlayerCharacter : public jt::Box2DObject {
public:
    PlayerCharacter(
        std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state);

    std::shared_ptr<CharacterSheetImgui> getCharSheet();
    void handleInputMovement();
    void updateAnimation(float const elapsed);

    void gainExperience(int value);

    void receiveDamage(Damage const& dmg);

    void die();
    bool m_hasFinishedDying { false };

private:
    StateGame& m_state;

    std::shared_ptr<jt::Animation> m_animation;
    std::shared_ptr<jt::Animation> m_attackUnderlay;
    std::shared_ptr<CharacterSheetImgui> m_charsheet;

    std::shared_ptr<jt::Sound> m_soundDash;
    std::shared_ptr<jt::Sound> m_soundStomp;
    std::shared_ptr<jt::Sound> m_soundDeath;

    std::shared_ptr<jt::SoundGroup> m_soundGroupHurt;

    std::shared_ptr<SpellBook> m_spellBook;

    float m_dashTimer { -1.0f };
    float m_dashCooldown { -1.0f };
    jt::Vector2f m_dashVelocity { 0.0f, 0.0f };

    float m_attackCooldown { -1.0f };

    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void handleDashInput();
    void createAnimation();
    void handleInputAttack();
    std::string selectDashAnimation(jt::Vector2f const& velocity) const;

    void updateSpells(const float elapsed);
    void updateOneSpell(
        float const elapsed, std::shared_ptr<SpellInterface> spell, jt::KeyCode key);

    bool setAnimationIfNotSet(std::string const& newAnimationName);
    bool m_isDying { false };
};

#endif // GUARD_JAMTEMPLATE_CHARACTER_HPP
