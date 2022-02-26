#include "enemy.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
Enemy::Enemy(std::shared_ptr<jt::Box2DWorldInterface> world, const b2BodyDef* def)
    : Box2DObject { world, def }
{
}
void Enemy::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();
    m_animation->add("assets/enemy_test.png", "idle", jt::Vector2u { 16, 16 }, { 0 }, 0.5f,
        getGame()->gfx().textureManager());
    m_animation->play("idle");

    b2FixtureDef fixtureDef;
    b2PolygonShape boxCollider {};
    boxCollider.SetAsBox(GP::PlayerSize().x / 2.0f, GP::PlayerSize().y / 2.0f);
    fixtureDef.shape = &boxCollider;
    getB2Body()->CreateFixture(&fixtureDef);
}
void Enemy::doUpdate(const float elapsed)
{
    m_animation->setPosition(getPosition() - GP::PlayerSize() * 0.5f);
    m_animation->update(elapsed);
}
void Enemy::doDraw() const { m_animation->draw(getGame()->gfx().target()); }
