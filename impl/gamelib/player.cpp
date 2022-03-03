#include "player.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "hud/hud.hpp"
#include "math_helper.hpp"
#include "spells/spell_none.hpp"
#include "spells/spell_passive_movement_speed.hpp"
#include "state_game.hpp"

Player::Player(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state)
    : jt::Box2DObject { world, def }
    , m_state { state }
{
}

void Player::doCreate()
{
    b2FixtureDef fixtureDef;
    b2PolygonShape boxCollider {};
    boxCollider.SetAsBox(GP::PlayerSize().x / 2.1f, GP::PlayerSize().y / 2.1f);
    fixtureDef.shape = &boxCollider;
    fixtureDef.filter.categoryBits = GP::PhysicsCollisionCategoryPlayer();
    fixtureDef.filter.maskBits = GP::PhysicsCollisionCategoryWalls()
        | GP::PhysicsCollisionCategoryEnemyShots() | GP::PhysicsCollisionCategoryEnemies();
    getB2Body()->CreateFixture(&fixtureDef);

    createAnimation();

    CharSheetObservers charSheetObservers { m_state.m_hud->getObserverExperience(),
        m_state.m_hud->getObserverHealth(), m_state.m_hud->getObserverHealthMax(), m_healCallback };

    m_charsheet = std::make_shared<CharacterSheetImgui>(charSheetObservers);
    m_charsheet->setGameInstance(getGame());
    m_charsheet->create();

    m_spellBook = std::make_shared<SpellBook>(m_state);
    m_spellBook->setGameInstance(getGame());
    m_spellBook->create();

    createSounds();

    m_commands.push_back(getGame()->getActionCommandManager().registerTemporaryCommand(
        "learnspell", [this](std::vector<std::string> args) {
            getGame()->cheat();
            if (args.size() != 1) {
                return;
            }
            m_spellBook->makeSpellAvailable(args.at(0));
        }));

    m_commands.push_back(getGame()->getActionCommandManager().registerTemporaryCommand(
        "getxp", [this](std::vector<std::string> args) {
            getGame()->cheat();
            if (args.size() != 1) {
                return;
            }
            int amount = std::stoi(args.at(0));
            m_charsheet->changeExperiencePoints(amount);
        }));
}
void Player::createSounds()
{
    m_soundDash = std::make_shared<jt::Sound>("assets/sound/attack_dash_3.ogg");
    m_soundDash->setVolume(0.4f);
    getGame()->audio().addTemporarySound(m_soundDash);

    m_soundStomp = std::make_shared<jt::Sound>("assets/sound/attack_stomp.ogg");
    m_soundStomp->setVolume(0.4f);
    getGame()->audio().addTemporarySound(m_soundStomp);

    m_soundDeath = std::make_shared<jt::Sound>("assets/sound/GAME_OVER.ogg");
    getGame()->audio().addTemporarySound(m_soundDeath);

    auto const soundHurt1 = std::make_shared<jt::Sound>("assets/sound/hit_squishy_sound_01.ogg");
    auto const soundHurt2 = std::make_shared<jt::Sound>("assets/sound/hit_squishy_sound_02.ogg");
    auto const soundHurt3 = std::make_shared<jt::Sound>("assets/sound/hit_squishy_sound_03.ogg");
    auto const soundHurt4 = std::make_shared<jt::Sound>("assets/sound/hit_squishy_sound_04.ogg");
    auto const soundHurt5 = std::make_shared<jt::Sound>("assets/sound/hit_squishy_sound_05.ogg");

    soundHurt1->setVolume(0.6f);
    soundHurt2->setVolume(0.6f);
    soundHurt3->setVolume(0.6f);
    soundHurt4->setVolume(0.6f);
    soundHurt5->setVolume(0.6f);

    m_soundGroupHurt = std::make_shared<jt::SoundGroup>();
    m_soundGroupHurt->add(soundHurt1);
    m_soundGroupHurt->add(soundHurt2);
    m_soundGroupHurt->add(soundHurt3);
    m_soundGroupHurt->add(soundHurt4);
    m_soundGroupHurt->add(soundHurt5);
}

void Player::createAnimation()
{
    auto const frameTimeAttack = 0.05f;

    m_animation = std::make_shared<jt::Animation>();

    // General
    {
        m_animation->add("assets/player.png", "idle",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 30, 30, 31, 32, 33, 34, 35, 36 }, 0.25f, getGame()->gfx().textureManager());

        m_animation->add("assets/player.png", "attack_down",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 78, 79, 80, /*81, 82,*/ 83, 84, 85, 86, 87 }, frameTimeAttack,
            getGame()->gfx().textureManager());

        m_animation->add("assets/player.png", "attack_up",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 88, 89, 90, /*91, 92,*/ 93, 94, 95, 96 }, frameTimeAttack,
            getGame()->gfx().textureManager());

        m_animation->add("assets/player.png", "hurt",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 37, 38, 39, 40 }, 0.15f, getGame()->gfx().textureManager());

        m_animation->add("assets/player.png", "die",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 69, 70, 71, 72, 73, 74, 75, 76, 77 }, 0.15f, getGame()->gfx().textureManager());
    }

    // Movement
    {
        m_animation->add("assets/player.png", "right",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 25, 26, 27, 28, 29 }, 0.08f, getGame()->gfx().textureManager());

        m_animation->add("assets/player.png", "left",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 20, 21, 22, 23, 24 }, 0.08f, getGame()->gfx().textureManager());

        m_animation->add("assets/player.png", "up",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 15, 16, 17, 18, 19 }, 0.08f, getGame()->gfx().textureManager());

        m_animation->add("assets/player.png", "down",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 0, 1, 2, 3, 4 }, 0.08f, getGame()->gfx().textureManager());
    }

    // Movement diagonally
    {
        m_animation->add("assets/player.png", "down_right",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 10, 11, 12, 13, 14 }, 0.08f, getGame()->gfx().textureManager());

        m_animation->add("assets/player.png", "down_left",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 5, 6, 7, 8, 9 }, 0.08f, getGame()->gfx().textureManager());
    }

    // Dash
    {
        m_animation->add("assets/player.png", "dash_left",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 41, 42, 43, 44, 45 }, 0.1f, getGame()->gfx().textureManager());

        m_animation->add("assets/player.png", "dash_right",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 46, 47, 48, 49, 50 }, 0.1f, getGame()->gfx().textureManager());

        m_animation->add("assets/player.png", "dash_up",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 46, 47, 48, 49, 50 }, 0.1f, getGame()->gfx().textureManager());

        m_animation->add("assets/player.png", "dash_up_left",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 61, 62, 63, 64, 65 }, 0.1f, getGame()->gfx().textureManager());

        m_animation->add("assets/player.png", "dash_up_right",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 56, 57, 58, 59, 60 }, 0.1f, getGame()->gfx().textureManager());

        m_animation->add("assets/player.png", "dash_down",
            jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
                static_cast<unsigned int>(GP::PlayerSize().y) },
            { 51, 52, 53, 54, 55 }, 0.1f, getGame()->gfx().textureManager());
    }

    m_animation->play("idle");
    m_animation->setPosition(jt::Vector2f { 5 * 24, 7 * 24 });

    m_attackUnderlay = std::make_shared<jt::Animation>();
    m_attackUnderlay->setLooping(false);
    m_attackUnderlay->add("assets/attack_underlay.png", "initial", jt::Vector2u { 32u, 32u }, { 0 },
        0.1f, getGame()->gfx().textureManager());
    m_attackUnderlay->add("assets/attack_underlay.png", "attack", jt::Vector2u { 32u, 32u },
        jt::MathHelper::numbersBetween(0u, 9u), frameTimeAttack * 2.0f,
        getGame()->gfx().textureManager());
    m_attackUnderlay->play("initial");
}

void Player::doUpdate(float const elapsed)
{
    handleInputMovement();
    handleInputAttack();

    updateSpells(elapsed);
    updateAnimation(elapsed);

    m_dashTimer -= elapsed;
    m_dashCooldown -= elapsed * m_charsheet->getDashFactor();
    m_attackCooldown -= elapsed * m_charsheet->getAttackSpeedFactor();

    m_charsheet->update(elapsed);
    m_spellBook->update(elapsed);

    if (getCharSheet()->getHitpoints() <= 0 && !m_soundDeath->isPlaying() && m_isDying) {
        m_hasFinishedDying = true;
    }
}

void Player::updateSpells(const float elapsed)
{
    auto const& equippedSpells = m_spellBook->getEquippedSpells();

    auto equippedSpellTexts = m_spellBook->getEquippedSpellTexts();
    updateOneSpell(elapsed, equippedSpells.at(0), equippedSpellTexts.at(0),
        { jt::KeyCode::Q, jt::KeyCode::Num1, jt::KeyCode::Numpad1 });

    updateOneSpell(elapsed, equippedSpells.at(1), equippedSpellTexts.at(1),
        { jt::KeyCode::E, jt::KeyCode::Num2, jt::KeyCode::Numpad2 });

    updateOneSpell(elapsed, equippedSpells.at(2), equippedSpellTexts.at(2),
        { jt::KeyCode::Tab, jt::KeyCode::Num3, jt::KeyCode::Numpad3 });
}

void Player::updateOneSpell(float const elapsed, std::shared_ptr<SpellInterface> spell,
    std::shared_ptr<jt::Text> text, std::vector<jt::KeyCode> keys)
{
    spell->update(elapsed);
    auto const cost = spell->getExperienceCost();
    auto hasEnoughMana = m_charsheet->getExperiencePoints() >= cost;
    auto spellCanBeCast = spell->canTrigger() && hasEnoughMana;
    if (spellCanBeCast) {
        text->setColor(jt::colors::White);
    } else {
        text->setColor(jt::colors::Gray);
    }
    for (auto key : keys) {
        if (getGame()->input().keyboard()->justPressed(key)) {
            if (spellCanBeCast) {
                getGame()->getLogger().debug("Spell triggered: " + spell->getName());
                m_charsheet->changeExperiencePoints(-cost);
                spell->trigger();
            } else {
                text->shake(0.4f, 2);
            }
        }
    }
}

void Player::handleInputAttack()
{
    if (m_attackCooldown > 0.0f) {
        return;
    }
    if (m_dashTimer > 0.0f) {
        return;
    }
    if (getGame()->input().keyboard()->justPressed(jt::KeyCode::Space)
        || getGame()->input().keyboard()->justPressed(jt::KeyCode::Numpad0)) {
        m_attackCooldown = GP::PlayerAttackCooldown();
    }
}

void Player::updateAnimation(float const elapsed)
{
    auto const v = getVelocity();
    if (m_dashTimer > 0.0f) {
        auto const dashAnimationName = selectDashAnimation(v);

        if (setAnimationIfNotSet(dashAnimationName)) {
            m_soundDash->stop();
            m_soundDash->play();

            m_animation->flash(0.3f);
            auto p = getPosition();
            auto vn = v;
            jt::MathHelper::normalizeMe(vn);
            m_dashVelocity = vn * GP::PlayerBaseDashVelocity() * m_charsheet->getDashFactor();

            // TODO trigger eye candy
        }
        if (m_dashTimer >= GP::PlayerDashTotalTime()
                - GP::PlayerDashActiveTime() * m_charsheet->getDashFactor()) {
            setVelocity(m_dashVelocity);
        } else {
            setVelocity(jt::Vector2f { 0.0f, 0.0f });
        }

    } else if (m_attackCooldown > 0.0f) {
        if (setAnimationIfNotSet("attack_down")) {
            m_soundStomp->stop();
            m_soundStomp->play();

            m_attackUnderlay->play("attack", 0, true);
            // TODO: Spore particle effects or whatev

            for (auto enemyWk : *m_state.getEnemies()) {
                auto enemy = enemyWk.lock();
                if (enemy == nullptr) {
                    continue;
                }
                auto nmePos = enemy->getPosition();
                auto myPos = getPosition();
                jt::Vector2f delta = nmePos - myPos;
                auto dist = jt::MathHelper::length(delta);
                // TODO: Maybe derive this from gear
                float circularHurtboxRange = 20.0f;
                float directedHurtboxRange = 45.0f;

                if (dist < directedHurtboxRange) {
                    // Forward-facing hurtbox with medium range
                    jt::Vector2f look = getVelocity();
                    if (jt::MathHelper::length(look) < 0.1f) {
                        // Look down if not moving
                        look = { 0.0f, 1.0f };
                    }
                    jt::MathHelper::normalizeMe(look);
                    jt::MathHelper::normalizeMe(delta);
                    float sc = jt::MathHelper::dot(look, delta);
                    if (sc > 0.5f) {
                        enemy->receiveDamage(Damage { m_charsheet->getAttackDamageValue() });
                    }
                    continue;
                } else if (dist < circularHurtboxRange) {
                    // Circular hurtbox with short range
                    enemy->receiveDamage(Damage { m_charsheet->getAttackDamageValue() / 3.0f });
                }
            }
        }
    } else {
        // no dash, no attack
        if (jt::MathHelper::lengthSquared(v) < 2) {
            setAnimationIfNotSet("idle");
        } else if (abs(v.x) > abs(v.y)) {
            if (v.x > 0) {
                setAnimationIfNotSet("right");
            } else {
                setAnimationIfNotSet("left");
            }
        } else {
            if (v.y > 0 && abs(v.x) < 0.1f) {
                setAnimationIfNotSet("down");
            } else if (v.y > 0 && v.x > 0) {
                setAnimationIfNotSet("down_right");
            } else if (v.y > 0 && v.x < 0) {
                setAnimationIfNotSet("down_left");
            } else {
                setAnimationIfNotSet("up");
            }
        }
    }

    m_animation->setPosition(getPosition() - GP::PlayerSize() * 0.5f);
    m_attackUnderlay->setPosition(
        getPosition() - GP::PlayerSize() * 0.5f - jt::Vector2f { 8.0f, 8.0f });

    m_animation->update(elapsed);
    m_attackUnderlay->update(elapsed);
}

std::string Player::selectDashAnimation(jt::Vector2f const& velocity) const
{
    std::string dashAnimationName { "dash_down" };
    if (velocity.x > 0 && abs(velocity.y) >= 0 && abs(velocity.y) < 0.1f) {
        dashAnimationName = "dash_right";
    } else if (velocity.x < 0 && abs(velocity.y) >= 0 && abs(velocity.y) < 0.1f) {
        dashAnimationName = "dash_left";
    } else if (velocity.x > 0 && velocity.y < 0) {
        dashAnimationName = "dash_up_right";
    } else if (velocity.x < 0 && velocity.y < 0) {
        dashAnimationName = "dash_up_left";
    } else if (abs(velocity.x) >= 0 && abs(velocity.x) < 0.1f && velocity.y < 0) {
        dashAnimationName = "dash_up";
    }
    return dashAnimationName;
}

bool Player::setAnimationIfNotSet(std::string const& newAnimationName)
{
    std::string const& currentAnimationName = m_animation->getCurrentAnimationName();

    if (currentAnimationName == "die") {
        return true;
    }

    if (currentAnimationName == "hurt" && newAnimationName == "idle") {
        return true;
    }

    if (currentAnimationName != newAnimationName) {
        m_animation->play(newAnimationName);
        return true;
    }
    return false;
}

void Player::handleInputMovement()
{
    auto keyboard = getGame()->input().keyboard();

    if (m_dashTimer < 0.0f) {
        setVelocity(jt::Vector2f { 0.0f, 0.0f });
        float const speed = GP::PlayerBaseMovementSpeed() * m_charsheet->getMovementSpeedFactor();

        if (keyboard->pressed(jt::KeyCode::D) || keyboard->pressed(jt::KeyCode::Right)) {
            addVelocity(jt::Vector2f { speed, 0.0f });
        }
        if (keyboard->pressed(jt::KeyCode::A) || keyboard->pressed(jt::KeyCode::Left)) {
            addVelocity(jt::Vector2f { -speed, 0.0f });
        }

        if (keyboard->pressed(jt::KeyCode::W) || keyboard->pressed(jt::KeyCode::Up)) {
            addVelocity(jt::Vector2f { 0.0f, -speed });
        }
        if (keyboard->pressed(jt::KeyCode::S) || keyboard->pressed(jt::KeyCode::Down)) {
            addVelocity(jt::Vector2f { 0.0f, speed });
        }
    }

    if (keyboard->justPressed(jt::KeyCode::LShift) || keyboard->pressed(jt::KeyCode::RShift)) {
        handleDashInput();
    }
}
void Player::handleDashInput()
{
    if (m_dashCooldown >= 0) {
        return;
    }
    if (jt::MathHelper::lengthSquared(getVelocity()) > 0) {
        m_dashTimer = GP::PlayerDashTotalTime();
        m_dashCooldown = GP::PlayerBaseDashCooldown();
    }
}

void Player::doDraw() const
{
    m_attackUnderlay->draw(getGame()->gfx().target());
    m_animation->draw(getGame()->gfx().target());
    m_charsheet->draw();
    m_spellBook->draw();
}

std::shared_ptr<CharacterSheetImgui> Player::getCharSheet() { return m_charsheet; }

void Player::gainExperience(int value) { m_charsheet->changeExperiencePoints(value); }

void Player::receiveDamage(Damage const& dmg)
{
    m_charsheet->changeHitpoints(dmg.value);
    setAnimationIfNotSet("hurt");
    m_soundGroupHurt->play();
}

void Player::die()
{
    if (!m_isDying) {
        m_isDying = true;
        m_soundDeath->play();
        m_animation->setLooping(false);
        setAnimationIfNotSet("die");
    }
}
std::shared_ptr<SpellBook> Player::getSpellBook() { return m_spellBook; }

jt::Vector2f Player::getTargetPosition() { return getPosition(); }
void Player::applyDamageToTarget(Damage const& dmg) { receiveDamage(dmg); }

void Player::setHealCallback(std::function<void(void)> healCallback)
{
    m_healCallback = [healCallback, this]() {
        m_animation->flash(0.2f, jt::colors::Green);
        healCallback();
    };
}
