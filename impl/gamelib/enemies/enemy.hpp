#ifndef GUARD_JAMTEMPLATE_ENEMY_GUARD_HPP
#define GUARD_JAMTEMPLATE_ENEMY_GUARD_HPP

#include "box2dwrapper/box2d_object.hpp"
#include "deferred_action_interface.hpp"
#include "enemy_ai/ai_state_manager.hpp"
#include "enemy_info.hpp"
#include "experience_spawner_interface.hpp"
#include "target_interface.hpp"
#include "world_path_calculator_interface.hpp"
#include <memory>

class StateGame;
class ProjectileSpawnerInterface;
struct Damage;

namespace jt {
class Animation;
class SoundInterface;
class Bar;
} // namespace jt

// TODO compose class via mix ins?
class Enemy : public jt::Box2DObject, public TargetInterface {
public:
    Enemy(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def);
    Enemy(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def,
        EnemyInfo const& info);
    void receiveDamage(Damage const& dmg);

    void setTarget(std::weak_ptr<TargetInterface> target);
    void setPathCalculator(WorldPathCalculatorInterface* calculator);
    void setProjectileSpawner(ProjectileSpawnerInterface* spawner);
    void setDeferredActionHandler(DeferredActionInterface* handler);
    void setExperienceSpawner(ExperienceSpawnerInterface* spawner);

    void gainExperience(int value) override;

    AiStateManager& getAiStateManager();
    void moveInDirection(jt::Vector2f const& dir);
    float playAnimation(std::string const& animName);

    float getCloseCombatDamage() const;

    jt::Vector2f getTargetPosition() override;
    void applyDamageToTarget(Damage const& dmg) override;
    bool isBoss();
    void makeSpellAvailable(std::string const& spellName) override;

    float getHitpoints() const;

    void drawHud() const;

private:
    float m_hitpoints { 1.0f };
    int m_experience { 0 };
    float m_attackCooldown { -1.0f };
    float m_movementSpeed { 1.0f };
    float m_closeCombatDamage { 0.0f };
    EnemyInfo m_info;

public:
    EnemyInfo const& getInfo() const;

private:
    std::shared_ptr<jt::Animation> m_animation;
    std::shared_ptr<jt::Bar> m_bar;

    std::shared_ptr<jt::SoundInterface> m_soundHit;

    AiStateManager m_aiStateManager;
    float m_staggeredTime { -1.0f };

    // non owning weak or raw pointers
    std::weak_ptr<TargetInterface> m_target;
    WorldPathCalculatorInterface* m_pathCalculator { nullptr };
    ProjectileSpawnerInterface* m_projectileSpawner { nullptr };

    bool m_isInDieAnimation { false };
    jt::Vector2f m_deathPosition { 0.0f, 0.0f };

    // non owning weak or raw pointers
    ExperienceSpawnerInterface* m_experienceSpawner { nullptr };
    DeferredActionInterface* m_deferredActionHandler { nullptr };

    bool m_isBoss { false };

    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;

    void performAI(float elapsed);
    void die();
};
#endif
