#include "enemy.hpp"
#include "animation.hpp"
#include "audio/sound.hpp"
#include "damage.hpp"
#include "enemies/enemy_ai/ai_state_boss.hpp"
#include "enemies/enemy_ai/ai_state_follow_target.hpp"
#include "enemies/enemy_ai/ai_state_shooter.hpp"
#include "enemies/enemy_ai/ai_state_wait_for_target.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "projectile_spawner_interface.hpp"
#include "system_helper.hpp"

Enemy::Enemy(std::shared_ptr<jt::Box2DWorldInterface> world, const b2BodyDef* def)
    : Box2DObject { world, def }
{
}

Enemy::Enemy(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, EnemyInfo const& info)
    : Box2DObject { world, def }
{
    m_info = info;
    m_experience = info.experience;
    m_hitpoints = info.hitpoints;
    m_closeCombatDamage = info.closeCombatDamage;
    m_movementSpeed = info.movementSpeed;
    m_isBoss = info.isBoss;
}

void Enemy::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();
    for (auto const& animInfo : m_info.animations) {
        m_animation->add(animInfo.fileName, animInfo.animationName, animInfo.imageSize,
            animInfo.frameIndices, animInfo.frameTimeInSeconds, getGame()->gfx().textureManager());
    }
    m_animation->play(m_info.animations.begin()->animationName);

    b2FixtureDef fixtureDef;
    b2CircleShape circle {};
    circle.m_radius = m_info.colliderRadius;

    fixtureDef.shape = &circle;
    fixtureDef.filter.categoryBits = GP::PhysicsCollisionCategoryEnemies();
    fixtureDef.filter.maskBits = GP::PhysicsCollisionCategoryWalls()
        | GP::PhysicsCollisionCategoryPlayer() | GP::PhysicsCollisionCategoryPlayerShots();

    getB2Body()->CreateFixture(&fixtureDef);

    for (auto const& aiInfo : m_info.ais) {
        if (aiInfo.type == aiInfo.WAIT) {
            auto waitState = std::make_shared<AiStateWaitForTarget>();
            waitState->setTarget(m_target);
            waitState->setNextState(aiInfo.nextState);
            waitState->setDetectRange(aiInfo.range);
            getAiStateManager().registerState(aiInfo.name, waitState);
        } else if (aiInfo.type == aiInfo.SHOOT) {
            auto shooterState = std::make_shared<AiStateShooter>();
            shooterState->setTarget(m_target);
            shooterState->setNextState(aiInfo.nextState);
            shooterState->setForgetRange(aiInfo.range);
            shooterState->setProjectileSpawner(m_projectileSpawner);
            getAiStateManager().registerState(aiInfo.name, shooterState);
        } else if (aiInfo.type == aiInfo.FOLLOW) {
            auto followState = std::make_shared<AiStateFollowTarget>();
            followState->setTarget(m_target);
            followState->setNextState(aiInfo.nextState);
            followState->setPathCalculator(m_pathCalculator);
            getAiStateManager().registerState(aiInfo.name, followState);
        } else if (aiInfo.type == aiInfo.BOSS) {
            auto bossState = std::make_shared<AiStateBoss>();
            bossState->setTarget(m_target);
            bossState->setPathCalculator(m_pathCalculator);
            bossState->setProjectileSpawner(m_projectileSpawner);
            getAiStateManager().registerState(aiInfo.name, bossState);
        }
    }
    getAiStateManager().switchToState(m_info.ais.begin()->name);
}

void Enemy::doUpdate(const float elapsed)
{
    m_staggeredTime -= elapsed;
    m_attackCooldown -= elapsed;

    m_animation->update(elapsed);
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
}

void Enemy::doDraw() const { m_animation->draw(getGame()->gfx().target()); }

void Enemy::receiveDamage(const Damage& dmg)
{
    // TODO visual candy
    m_animation->flash(0.2f, jt::Color { 97, 16, 162 });

    m_hitpoints -= dmg.value;

    m_staggeredTime = 0.25f;
    setVelocity({ 0.0f, 0.0f });
    if (m_hitpoints <= 0.0f) {
        die();
    }
}

void Enemy::die()
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
}

void Enemy::setTarget(std::weak_ptr<TargetInterface> target) { m_target = target; }
void Enemy::setPathCalculator(WorldPathCalculatorInterface* calculator)
{
    m_pathCalculator = calculator;
}

void Enemy::setProjectileSpawner(ProjectileSpawnerInterface* spawner)
{
    m_projectileSpawner = spawner;
}

void Enemy::performAI(float elapsed)
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

void Enemy::setDeferredActionHandler(DeferredActionInterface* handler)
{
    m_deferredActionHandler = handler;
}

void Enemy::setExperienceSpawner(ExperienceSpawnerInterface* spawner)
{
    m_experienceSpawner = spawner;
}

AiStateManager& Enemy::getAiStateManager() { return m_aiStateManager; }
void Enemy::moveInDirection(jt::Vector2f const& dir) { setVelocity(dir * m_movementSpeed); }
float Enemy::playAnimation(std::string const& animName)
{
    m_animation->play(animName);
    return m_animation->getCurrentAnimTotalTime();
}
float Enemy::getCloseCombatDamage() const { return m_closeCombatDamage; }
jt::Vector2f Enemy::getTargetPosition() { return getPosition(); }
void Enemy::applyDamageToTarget(Damage const& dmg) { receiveDamage(dmg); }
void Enemy::gainExperience(int value)
{ /* noting to do*/
}
bool Enemy::isBoss() { return m_isBoss; }
void Enemy::makeSpellAvailable(std::string const& spellName) { }
