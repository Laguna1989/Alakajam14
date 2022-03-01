#ifndef GUARD_JAMTEMPLATE_ENEMY_GUARD_HPP
#define GUARD_JAMTEMPLATE_ENEMY_GUARD_HPP

#include "animation.hpp"
#include "audio/sound.hpp"
#include "box2dwrapper/box2d_object.hpp"
#include "damage.hpp"
#include "target_interface.hpp"
#include "world_path_calculator_interface.hpp"

class StateGame;

class EnemyBase : public jt::Box2DObject {
public:
    EnemyBase(
        std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state);
    void receiveDamage(Damage const& dmg);

    void setTarget(std::weak_ptr<TargetInterface> target);
    void setPathCalculator(WorldPathCalculatorInterface* calculator);

protected:
    float m_hitpoints { 1.0f };
    int m_experience { 0 };
    float m_attackCooldown { -1.0f };
    std::shared_ptr<jt::Animation> m_animation;
    StateGame& m_state;

    // non owning weak or raw pointers
    std::weak_ptr<TargetInterface> m_target;
    WorldPathCalculatorInterface* m_pathCalculator;

    bool m_isInDieAnimation { false };
    jt::Vector2f m_deathPosition { 0.0f, 0.0f };

    bool canAttack() const;

private:
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;

    void performAI(float elapsed);
    virtual void doPerformAI(float elapsed) = 0;
    void die();
    virtual void doDie() {};
};
#endif
