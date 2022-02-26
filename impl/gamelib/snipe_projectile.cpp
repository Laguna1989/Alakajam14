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
}

void SnipeProjectile::doUpdate(float const elapsed)
{
    m_shape->setPosition(getPosition());
    m_shape->update(elapsed);
}

void SnipeProjectile::doDraw() const { m_shape->draw(getGame()->gfx().target()); }
