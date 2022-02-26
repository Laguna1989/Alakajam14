#ifndef ALAKAJAM14_SNIPE_PROJECTILE_HPP
#define ALAKAJAM14_SNIPE_PROJECTILE_HPP

#include "box2dwrapper/box2d_object.hpp"
#include "damage.hpp"
#include "graphics/drawable_interface.hpp"
#include <memory>

class SnipeProjectile : public jt::Box2DObject {
public:
    SnipeProjectile(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def);

    void setDamage(Damage const& d);
    Damage getDamage() const;

    virtual void doCreate();
    virtual void doUpdate(float const elapsed);
    virtual void doDraw() const;

private:
    std::shared_ptr<jt::DrawableInterface> m_shape;
    Damage m_damage;
};

#endif // ALAKAJAM14_SNIPE_PROJECTILE_HPP
