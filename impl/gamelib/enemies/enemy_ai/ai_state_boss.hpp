#ifndef ALAKAJAM14_AI_STATE_BOSS_HPP
#define ALAKAJAM14_AI_STATE_BOSS_HPP

#include "ai_state_follow_target.hpp"
#include "ai_state_impl.hpp"
#include "ai_state_shooter.hpp"
#include <string>

class AiStateBoss : public AiStateImpl {
public:
    AiStateBoss();
    void update(float elapsed, Enemy* base) override;
    void setPathCalculator(WorldPathCalculatorInterface* mpathCalculator);
    void setProjectileSpawner(ProjectileSpawnerInterface* spawner);
    void setNextState(std::string const& nextState);

private:
    ProjectileSpawnerInterface* m_projectileSpawner { nullptr };
    float m_shootTimer { -1.0f };
    void handleShooting(float elapsed);
    void checkForStateSwitch(Enemy* base);
    std::string m_nextState = { "" };
};

#endif // ALAKAJAM14_AI_STATE_BOSS_HPP
