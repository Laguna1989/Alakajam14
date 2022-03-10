#ifndef ALAKAJAM14_AI_STATE_BOSS_HPP
#define ALAKAJAM14_AI_STATE_BOSS_HPP

#include "ai_state_follow_target.hpp"
#include "ai_state_impl.hpp"
#include "ai_state_shooter.hpp"

class AiStateBoss : public AiStateImpl {
public:
    AiStateBoss();
    void update(float elapsed, Enemy* base) override;
    void setPathCalculator(WorldPathCalculatorInterface* mpathCalculator);
    void setProjectileSpawner(ProjectileSpawnerInterface* spawner);

private:
    ProjectileSpawnerInterface* m_projectileSpawner { nullptr };
};

#endif // ALAKAJAM14_AI_STATE_BOSS_HPP
