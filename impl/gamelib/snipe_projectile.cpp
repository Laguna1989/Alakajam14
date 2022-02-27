#include "snipe_projectile.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "shape.hpp"

SnipeProjectile::SnipeProjectile(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def)
    : Box2DObject(world, def)
{
    m_damage = { 200.0f };
}

void SnipeProjectile::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();
    m_animation->add("assets/player_projectile.png", "idle", jt::Vector2u { 16u, 16u },
        { 0, 1, 2, 3, 4 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->play("idle");
    m_animation->setRotation(m_rotation);

    b2FixtureDef fixtureDef;
    b2PolygonShape boxCollider {};
    boxCollider.SetAsBox(8.0f, 8.0f);
    fixtureDef.shape = &boxCollider;
    getB2Body()->CreateFixture(&fixtureDef);
}

void SnipeProjectile::doUpdate(float const elapsed)
{
    m_animation->setPosition(getPosition()
        - jt::Vector2f { m_animation->getLocalBounds().width, m_animation->getLocalBounds().height }
            / 2.0f);
    m_animation->update(elapsed);
}

void SnipeProjectile::doDraw() const { m_animation->draw(getGame()->gfx().target()); }
Damage SnipeProjectile::getDamage() const { return m_damage; }

void SnipeProjectile::setDamage(Damage const& d) { m_damage = d; }
void SnipeProjectile::setRotation(float rotation) { m_rotation = rotation; }
