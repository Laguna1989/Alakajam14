#ifndef ALAKAJAM14_AI_STATE_SHOOTER_HPP
#define ALAKAJAM14_AI_STATE_SHOOTER_HPP

#include "ai_state_impl.hpp"
#include <string>

class ProjectileSpawnerInterface;

class AiStateShooter : public AiStateImpl {
public:
    void update(float elapsed, EnemyBase* base) override;
    void setNextState(std::string const& nextState);
    void setForgetRange(float forgetRange);
    void setProjectileSpawner(ProjectileSpawnerInterface* spawner);

private:
    std::string m_nextState = { "" };
    float m_forgetRange { 180.0f };
    float m_shootTimer { -1.0f };
    ProjectileSpawnerInterface* m_projectileSpawner { nullptr };
};

#endif // ALAKAJAM14_AI_STATE_SHOOTER_HPP
