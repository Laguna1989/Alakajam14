#ifndef ALAKAJAM14_AI_STATE_BOSS_CRITICAL_HPP
#define ALAKAJAM14_AI_STATE_BOSS_CRITICAL_HPP

#include "ai_state_follow_target.hpp"
#include "ai_state_impl.hpp"
#include "ai_state_shooter.hpp"

class AiStateBossCritical : public AiStateImpl {
public:
    AiStateBossCritical();
    void update(float elapsed, Enemy* base) override;
    void setPathCalculator(WorldPathCalculatorInterface* mpathCalculator);
    void setProjectileSpawner(ProjectileSpawnerInterface* spawner);

private:
    ProjectileSpawnerInterface* m_projectileSpawner { nullptr };
    float m_shootTimer { -1.0f };
};

#endif // ALAKAJAM14_AI_STATE_BOSS_CRITICAL_HPP
