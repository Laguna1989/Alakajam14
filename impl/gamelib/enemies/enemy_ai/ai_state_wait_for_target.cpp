#include "ai_state_wait_for_target.hpp"
#include "enemies/enemy.hpp"

void AiStateWaitForTarget::update(float elapsed, Enemy* base)
{
    float const distanceSquared = calculateDistanceToTarget();

    if (distanceSquared < m_detectRange * m_detectRange) {
        base->getAiStateManager().switchToState(m_nextState);
        return;
    }
}

void AiStateWaitForTarget::setNextState(std::string const& nextState) { m_nextState = nextState; }
void AiStateWaitForTarget::setDetectRange(float range) { m_detectRange = range; }
