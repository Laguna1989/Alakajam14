#ifndef GUARD_JAMTEMPLATE_ENEMY_GUARD_HPP
#define GUARD_JAMTEMPLATE_ENEMY_GUARD_HPP
#include "animation.hpp"
#include "box2dwrapper/box2d_object.hpp"
#include "damage.hpp"

class StateGame;

class EnemyBase : public jt::Box2DObject {
public:
    EnemyBase(
        std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state);
    void receiveDamage(Damage const& dmg);

protected:
    float m_hitpoints;
    float m_experience;
    std::shared_ptr<jt::Animation> m_animation;
    StateGame& m_state;

private:
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;

    virtual void doAI(float elapsed) = 0;
    void die();
};
#endif
