#ifndef ALAKAJAM14_AI_STATE_FOLLOW_TARGET_HPP
#define ALAKAJAM14_AI_STATE_FOLLOW_TARGET_HPP

#include "ai_state_impl.hpp"
#include "pathfinder/node_interface.hpp"
#include <string>
#include <vector>

class WorldPathCalculatorInterface;

class AiStateFollowTarget : public AiStateImpl {
public:
    void update(float elapsed, Enemy* base) override;
    void setNextState(std::string const& nextState);
    void setForgetRange(float range);
    void setPathCalculator(WorldPathCalculatorInterface* pathCalculator);

private:
    std::string m_nextState = { "" };
    float m_forgetRange { 180.0f };

    WorldPathCalculatorInterface* m_pathCalculator { nullptr };
    float m_timeToPathfind { -1.0f };
    std::vector<std::shared_ptr<jt::pathfinder::NodeInterface>> m_cachedPath;
    void walkToTarget(float elapsed, Enemy* base);

    void closeRangeAttack(float elapsed, Enemy* base);
    float m_timeSinceTriggeredAttack { -1.0f };
    float m_attackCooldown { -1.0f };
};

#endif // ALAKAJAM14_AI_STATE_FOLLOW_TARGET_HPP
