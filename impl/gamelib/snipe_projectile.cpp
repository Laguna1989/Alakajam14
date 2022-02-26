#include "snipe_projectile.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "shape.hpp"

SnipeProjectile::SnipeProjectile(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def)
    : Box2DObject(world, def)
{
}

void SnipeProjectile::doCreate()
{
    m_shape = jt::dh::createShapeRect(
        jt::Vector2f { 8.0f, 8.0 }, jt::colors::Red, getGame()->gfx().textureManager());
    b2FixtureDef fixtureDef;
    b2PolygonShape boxCollider {};
    boxCollider.SetAsBox(4.0f, 4.0f);
    fixtureDef.shape = &boxCollider;
    getB2Body()->CreateFixture(&fixtureDef);
}

void SnipeProjectile::doUpdate(float const elapsed)
{
    m_shape->setPosition(getPosition()
        - jt::Vector2f { m_shape->getLocalBounds().width, m_shape->getLocalBounds().height }
            / 2.0f);
    m_shape->update(elapsed);
}

void SnipeProjectile::doDraw() const { m_shape->draw(getGame()->gfx().target()); }
