#include "ai_state_follow_target.hpp"
#include "enemies/enemy.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "random/random.hpp"

void AiStateFollowTarget::setNextState(std::string const& nextState) { m_nextState = nextState; }
void AiStateFollowTarget::setForgetRange(float range) { m_forgetRange = range; }
void AiStateFollowTarget::setPathCalculator(WorldPathCalculatorInterface* pathCalculator)
{
    m_pathCalculator = pathCalculator;
}

void AiStateFollowTarget::update(float elapsed, Enemy* base)
{
    float const distanceSquared = calculateDistanceToTarget();
    if (distanceSquared > m_forgetRange * m_forgetRange) {
        if (m_nextState != "") {
            base->getAiStateManager().switchToState(m_nextState);
        }
        return;
    }

    walkToTarget(elapsed, base);
    closeRangeAttack(elapsed, base);
}

void AiStateFollowTarget::walkToTarget(float elapsed, Enemy* base)
{
    m_timeToPathfind -= elapsed;

    if (m_pathCalculator == nullptr) {
        return;
    }
    if (m_timeToPathfind <= 0) {
        m_cachedPath
            = m_pathCalculator->calculatePath(m_position, m_target.lock()->getTargetPosition());
        m_timeToPathfind = jt::Random::getFloat(0.4f, 0.5f);
    }
    if (m_cachedPath.size() < 2) {
        return;
    }

    auto const& nextTilePosition = m_cachedPath.at(1)->getTilePosition();

    jt::Vector2f const nextTilePositionFloat { nextTilePosition.x * GP::TileSizeInPixel() + 8,
        nextTilePosition.y * GP::TileSizeInPixel() + 8 };

    auto const enemyPosition = m_position;
    auto diffToTile = nextTilePositionFloat - enemyPosition;
    jt::MathHelper::normalizeMe(diffToTile);

    base->moveInDirection(diffToTile);
}

void AiStateFollowTarget::closeRangeAttack(float elapsed, Enemy* base)
{
    m_attackCooldown -= elapsed;

    auto const targetPosition = m_target.lock()->getTargetPosition();
    auto const enemyPosition = m_position;

    auto diff = targetPosition - enemyPosition;

    auto const distanceSquared = jt::MathHelper::lengthSquared(diff);
    if (m_timeSinceTriggeredAttack < 0.0f) {
        if (distanceSquared < 19 * 19) {
            if (m_attackCooldown <= 0.0f) {
                m_timeSinceTriggeredAttack = base->playAnimation("shoot");
            }
        }
    } else {
        m_timeSinceTriggeredAttack -= elapsed;
        base->moveInDirection(jt::Vector2f { 0.0f, 0.0f });
        if (m_timeSinceTriggeredAttack <= 0) {
            // TODO Visual candy
            if (distanceSquared < 22 * 22) {
                m_target.lock()->applyDamageToTarget(Damage { base->getCloseCombatDamage() });
                m_attackCooldown = 1.0f;
            }
            base->playAnimation("idle");
        }
    }
}
