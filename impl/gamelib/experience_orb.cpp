#include "experience_orb.hpp"
#include "animation.hpp"
#include "audio/sound.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "random/random.hpp"

ExperienceOrb::ExperienceOrb(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def,
    int value, std::weak_ptr<TargetInterface> target)
    : jt::Box2DObject { world, def }
    , m_target { target }
{
    m_value = value;
    m_animation = std::make_shared<jt::Animation>();
    m_animation->setScreenSizeHint(GP::GetScreenSize());
}

void ExperienceOrb::doCreate()
{
    float const frameTime = jt::Random::getFloat(0.18f, 0.22f);
    m_animation->add("assets/shards_red.png", "idle1", { 16, 16 }, { 0, 1, 2, 3 }, frameTime,
        getGame()->gfx().textureManager());
    m_animation->add("assets/shards_green.png", "idle2", { 16, 16 }, { 0, 1, 2, 3 }, frameTime,
        getGame()->gfx().textureManager());
    m_animation->add("assets/shards_blue.png", "idle3", { 16, 16 }, { 0, 1, 2, 3 }, frameTime,
        getGame()->gfx().textureManager());

    std::string animName = "idle" + std::to_string(jt::Random::getInt(2, 3));
    m_animation->play(animName);

    if (m_value >= 10) {
        m_animation->play("idle1");
    }

    m_animation->setScreenSizeHint(GP::GetScreenSize());

    b2FixtureDef fixtureDef;
    b2PolygonShape boxCollider {};
    boxCollider.SetAsBox(3.0f, 3.0f);
    fixtureDef.shape = &boxCollider;
    fixtureDef.filter.categoryBits = GP::PhysicsCollisionCategoryExperienceOrbs();
    fixtureDef.filter.maskBits = GP::PhysicsCollisionCategoryWalls();

    getB2Body()->CreateFixture(&fixtureDef);

    initSound();
}
void ExperienceOrb::initSound()
{
    m_soundBling = getGame()->audio().soundPool(
        "bling", []() { return std::make_shared<jt::Sound>("assets/sound/powerUp_bling.ogg"); }, 5);
    m_soundBling->setVolume(0.7f);
}

void ExperienceOrb::doUpdate(float const elapsed)
{
    m_animation->setPosition(getPosition() - GP::PlayerSize() * 0.5f);
    m_animation->update(elapsed);

    if (!m_soundBling->isPlaying() && m_pickedUp) {
        kill();
    }

    if (getAge() < GP::ExperienceOrbIdleTime()) {
        return;
    }
    auto const playerPosition = m_target->getTargetPosition();
    auto const orbPosition = getPosition();
    auto diff = playerPosition - orbPosition;
    auto const distance = jt::MathHelper::lengthSquared(diff);

    if (distance < GP::ExperienceOrbAttractDistance() * GP::ExperienceOrbAttractDistance()) {
        jt::MathHelper::normalizeMe(diff);
        setVelocity(diff * GP::ExperienceOrbVelocity());
    }
    if (distance < GP::ExperienceOrbPickupDistance() * GP::ExperienceOrbPickupDistance()
        && !m_pickedUp) {
        m_target->gainExperience(m_value);
        pickUp();
    }
}

void ExperienceOrb::doDraw() const { m_animation->draw(getGame()->gfx().target()); }

void ExperienceOrb::doKill() { }

void ExperienceOrb::pickUp()
{
    m_soundBling->play();
    m_pickedUp = true;
}
