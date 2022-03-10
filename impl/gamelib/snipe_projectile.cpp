#include "snipe_projectile.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "shape.hpp"

SnipeProjectile::SnipeProjectile(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def)
    : Box2DObject(world, def)
{
}

void SnipeProjectile::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();
    m_animation->add("assets/player_projectile.png", "idle", jt::Vector2u { 16u, 16u },
        { 0, 1, 2, 3, 4 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->play("idle");
    m_animation->setOrigin(jt::Vector2f { 8.0f, 8.0f });
    m_animation->setRotation(m_rotation);
    m_animation->setScreenSizeHint(GP::GetScreenSize());

    b2FixtureDef fixtureDef;
    b2PolygonShape boxCollider {};
    boxCollider.SetAsBox(3.0f, 3.0f);
    fixtureDef.shape = &boxCollider;
    fixtureDef.filter.categoryBits = GP::PhysicsCollisionCategoryPlayerShots();
    fixtureDef.filter.maskBits = GP::PhysicsCollisionCategoryWalls()
        | GP::PhysicsCollisionCategoryEnemies() | GP::PhysicsCollisionCategoryEnemyShots();

    getB2Body()->CreateFixture(&fixtureDef);
}

void SnipeProjectile::doUpdate(float const elapsed)
{
    m_animation->setPosition(getPosition());
    m_animation->update(elapsed);
}

void SnipeProjectile::doDraw() const { m_animation->draw(getGame()->gfx().target()); }
Damage SnipeProjectile::getDamage() const { return m_damage; }

void SnipeProjectile::setDamage(Damage const& d) { m_damage = d; }
void SnipeProjectile::setRotation(float rotation) { m_rotation = rotation; }
