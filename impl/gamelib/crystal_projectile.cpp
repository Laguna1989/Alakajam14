
#include "crystal_projectile.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"

CrystalProjectile::CrystalProjectile(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def)
    : Box2DObject(world, def)
{
}

void CrystalProjectile::setDamage(Damage const& d) { m_damage = d; }

void CrystalProjectile::setRotation(float rotation) { m_rotation = rotation; }

Damage CrystalProjectile::getDamage() const { return m_damage; }

void CrystalProjectile::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();
    m_animation->add("assets/crystal_shot.png", "idle", jt::Vector2u { 16u, 16u }, { 0, 1, 2, 3 },
        0.1f, getGame()->gfx().textureManager());
    m_animation->play("idle");

    b2FixtureDef fixtureDef;
    b2PolygonShape boxCollider {};
    boxCollider.SetAsBox(2.0f, 2.0f);
    fixtureDef.shape = &boxCollider;
    getB2Body()->CreateFixture(&fixtureDef);
}

void CrystalProjectile::doUpdate(float const elapsed)
{
    m_animation->setPosition(getPosition()
        - jt::Vector2f { m_animation->getLocalBounds().width, m_animation->getLocalBounds().height }
            / 2.0f);
    m_animation->update(elapsed);

    if (m_age >= 1.6f) {
        kill();
    }
}

void CrystalProjectile::doDraw() const { m_animation->draw(getGame()->gfx().target()); }
