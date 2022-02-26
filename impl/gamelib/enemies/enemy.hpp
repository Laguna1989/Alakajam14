#ifndef GUARD_JAMTEMPLATE_ENEMY_GUARD_HPP
#define GUARD_JAMTEMPLATE_ENEMY_GUARD_HPP
#include "animation.hpp"
#include "box2dwrapper/box2d_object.hpp"

class Enemy : public jt::Box2DObject {
public:
    Enemy(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def);

private:
    std::shared_ptr<jt::Animation> m_animation;

    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
};
#endif