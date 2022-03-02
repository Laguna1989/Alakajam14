#include "ai_state_impl.hpp"
#include "math_helper.hpp"
#include "target_interface.hpp"

void AiStateImpl::setTarget(std::weak_ptr<TargetInterface> target) { m_target = target; }
void AiStateImpl::setPosition(jt::Vector2f const& pos) { m_position = pos; }

float AiStateImpl::calculateDistanceToTarget() const
{
    auto const targetPosition = m_target.lock()->getTargetPosition();
    auto const enemyPosition = m_position;

    auto diff = targetPosition - enemyPosition;

    auto const distanceSquared = jt::MathHelper::lengthSquared(diff);
    return distanceSquared;
}
