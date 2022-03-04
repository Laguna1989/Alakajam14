#include "enemy_base.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "projectile_spawner_interface.hpp"
#include "system_helper.hpp"

EnemyBase::EnemyBase(std::shared_ptr<jt::Box2DWorldInterface> world, const b2BodyDef* def)
    : Box2DObject { world, def }
{
}

void EnemyBase::doUpdate(const float elapsed)
{
    m_staggeredTime -= elapsed;
    m_attackCooldown -= elapsed;
    if (!m_animation->isVisible()) {
        return;
    }

    if (!m_isInDieAnimation) {
        if (m_staggeredTime <= 0) {
            performAI(elapsed);
        }

        m_animation->setPosition(getPosition()
            - jt::Vector2f { m_animation->getLocalBounds().width,
                  m_animation->getLocalBounds().height }
                * 0.5f);

    } else {
        m_animation->setPosition(m_deathPosition
            - jt::Vector2f { m_animation->getLocalBounds().width,
                  m_animation->getLocalBounds().height }
                * 0.5f);
    }
    m_animation->update(elapsed);
}

void EnemyBase::doDraw() const { m_animation->draw(getGame()->gfx().target()); }

void EnemyBase::receiveDamage(const Damage& dmg)
{
    // TODO visual candy
    m_animation->flash(0.15f, jt::colors::Red);

    m_hitpoints -= dmg.value;

    m_staggeredTime = 0.25f;
    setVelocity({ 0.0f, 0.0f });
    if (m_hitpoints <= 0.0f) {
        die();
    }
}

void EnemyBase::die()
{
    // don't die twice
    if (m_isInDieAnimation) {
        return;
    }

    m_isInDieAnimation = true;
    m_animation->play("dead");
    m_deathPosition = getPosition();

    // move collider out of the way
    setPosition(jt::Vector2f { -9999999999.0f, -999999999999999.0f });

    if (m_deferredActionHandler == nullptr) {
        return;
    }

    m_deferredActionHandler->queueDeferredAction(m_animation->getCurrentAnimTotalTime(), [this]() {
        kill();
        if (m_experienceSpawner) {
            m_experienceSpawner->spawnExperience(m_experience, m_deathPosition, false);
        }
    });
    doDie();
}

void EnemyBase::setTarget(std::weak_ptr<TargetInterface> target) { m_target = target; }
void EnemyBase::setPathCalculator(WorldPathCalculatorInterface* calculator)
{
    m_pathCalculator = calculator;
}

void EnemyBase::setProjectileSpawner(ProjectileSpawnerInterface* spawner)
{
    m_projectileSpawner = spawner;
}

void EnemyBase::performAI(float elapsed)
{
    if (jt::SystemHelper::is_uninitialized_weak_ptr(m_target) || m_target.expired()) {
        return;
    }
    getAiStateManager().update();
    if (getAiStateManager().getCurrentState() == nullptr) {
        return;
    }

    auto currentState = getAiStateManager().getCurrentState();
    currentState->setPosition(getPosition());

    currentState->update(elapsed, this);
}

void EnemyBase::setDeferredActionHandler(DeferredActionInterface* handler)
{
    m_deferredActionHandler = handler;
}

void EnemyBase::setExperienceSpawner(ExperienceSpawnerInterface* spawner)
{
    m_experienceSpawner = spawner;
}

AiStateManager& EnemyBase::getAiStateManager() { return m_aiStateManager; }
void EnemyBase::moveInDirection(jt::Vector2f const& dir) { setVelocity(dir * m_movementSpeed); }
float EnemyBase::playAnimation(std::string const& animName)
{
    m_animation->play(animName);
    return m_animation->getCurrentAnimTotalTime();
}
float EnemyBase::getCloseCombatDamage() const { return m_closeCombatDamage; }
jt::Vector2f EnemyBase::getTargetPosition() { return getPosition(); }
void EnemyBase::applyDamageToTarget(Damage const& dmg) { receiveDamage(dmg); }
void EnemyBase::gainExperience(int value)
{ /* noting to do*/
}
