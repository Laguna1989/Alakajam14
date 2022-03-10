#ifndef ALAKAJAM14_AI_STATE_BOSS_INJURED_HPP
#define ALAKAJAM14_AI_STATE_BOSS_INJURED_HPP

#include "ai_state_follow_target.hpp"
#include "ai_state_impl.hpp"
#include "ai_state_shooter.hpp"

class AiStateBossInjured : public AiStateImpl {
public:
    AiStateBossInjured();
    void update(float elapsed, Enemy* base) override;
    void setPathCalculator(WorldPathCalculatorInterface* mpathCalculator);
    void setProjectileSpawner(ProjectileSpawnerInterface* spawner);
    void setNextState(std::string const& nextState);

private:
    ProjectileSpawnerInterface* m_projectileSpawner { nullptr };
    float m_shootTimer { -1.0f };
    bool m_oddShot { false };
    std::string m_nextState = { "" };
    void checkForStateSwitch(Enemy* base);
    void handleShooting(float elapsed);
};

#endif // ALAKAJAM14_AI_STATE_BOSS_INJURED_HPP
