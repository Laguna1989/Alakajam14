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
#include "input_component_interface.hpp"
#include "particle_system.hpp"
#include "shape.hpp"
#include "spells/spell_attack_snipe.hpp"
#include "spells/spell_book.hpp"
#include "target_interface.hpp"
#include <memory>
#include <string>

class StateGame;
class Player : public jt::Box2DObject, public TargetInterface {
public:
    Player(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state);

    std::shared_ptr<CharacterSheetImgui> getCharSheet();
    std::shared_ptr<SpellBook> getSpellBook();

    void updateAnimation(float const elapsed);

    void gainExperience(int value) override;

    void receiveDamage(Damage const& dmg);

    void die();
    bool m_hasFinishedDying { false };

    jt::Vector2f getTargetPosition() override;
    void applyDamageToTarget(Damage const& dmg) override;

    void setHealCallback(std::function<void(void)> healCallback);

    void dash();
    void attack();

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

    std::shared_ptr<InputComponentInterface> m_input { nullptr };

    float m_dashTimer { -1.0f };
    float m_dashCooldown { -1.0f };
    jt::Vector2f m_dashVelocity { 0.0f, 0.0f };

    float m_attackCooldown { -1.0f };

    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void createAnimation();
    std::string selectDashAnimation(jt::Vector2f const& velocity) const;

    void updateSpells(const float elapsed);
    void updateOneSpell(float const elapsed, std::shared_ptr<SpellInterface> spell,
        std::shared_ptr<jt::Text> text, std::vector<jt::KeyCode> keys);

    bool setAnimationIfNotSet(std::string const& newAnimationName);
    bool m_isDying { false };
    void createSounds();
    std::function<void(void)> m_healCallback;
    std::vector<std::shared_ptr<bool>> m_commands;
};

#endif // GUARD_JAMTEMPLATE_CHARACTER_HPP
