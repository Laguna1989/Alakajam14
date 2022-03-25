#include "healing_potion.hpp"
#include "animation.hpp"
#include "audio/sound.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "random/random.hpp"

HealingPotion::HealingPotion(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def,
    std::weak_ptr<Player> target)
    : jt::Box2DObject { world, def }
    , m_target { target }
{
    m_animation = std::make_shared<jt::Animation>();
    m_animation->setScreenSizeHint(GP::GetScreenSize());
}

void HealingPotion::doCreate()
{
    m_animation->add("assets/healing_potion.png", "idle", { 16, 16 }, { 0, 1, 2, 3 }, 0.2f,
        getGame()->gfx().textureManager());
    m_animation->play("idle");
    m_animation->setScreenSizeHint(GP::GetScreenSize());

    b2FixtureDef fixtureDef;
    b2PolygonShape boxCollider {};
    boxCollider.SetAsBox(3.0f, 3.0f);
    fixtureDef.shape = &boxCollider;
    fixtureDef.filter.categoryBits = GP::PhysicsCollisionCategoryHealingPotions();
    fixtureDef.filter.maskBits = GP::PhysicsCollisionCategoryWalls();

    getB2Body()->CreateFixture(&fixtureDef);

    initSound();
}
void HealingPotion::initSound()
{
    m_soundBling = getGame()->audio().getSoundFromSoundPool(
        "bling", []() { return std::make_shared<jt::Sound>("assets/sound/powerUp_bling.ogg"); }, 5);
    m_soundBling->setVolume(0.7f);
}

void HealingPotion::doUpdate(float const elapsed)
{
    m_animation->setPosition(getPosition() - GP::PlayerSize() * 0.5f);
    m_animation->update(elapsed);

    if (!m_soundBling->isPlaying() && m_pickedUp) {
        kill();
    }

    auto const playerPosition = m_target->getTargetPosition();
    auto const potPosition = getPosition();
    auto diff = playerPosition - potPosition;
    auto const distance = jt::MathHelper::lengthSquared(diff);

    if (distance < GP::HealingPotionPickupDistance() * GP::HealingPotionPickupDistance()
        && !m_pickedUp
        && m_target->getCharSheet()->getHitpoints()
            < m_target->getCharSheet()->getHitpointsMax() - 0.01f) {
        pickUp();
    }
}

void HealingPotion::doDraw() const { m_animation->draw(getGame()->gfx().target()); }

void HealingPotion::doKill() { }

void HealingPotion::pickUp()
{
    m_soundBling->play();
    m_target->healBy(static_cast<int>(GP::HealingPotionAmount()));
    m_pickedUp = true;
}
