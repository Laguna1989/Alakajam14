#include "enemy_base.hpp"
#include "character.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "state_game.hpp"

EnemyBase::EnemyBase(
    std::shared_ptr<jt::Box2DWorldInterface> world, const b2BodyDef* def, StateGame& state)
    : Box2DObject { world, def }
    , m_state { state }
{
}

void EnemyBase::doCreate()
{
    // TODO move into derived classes later
    m_animation = std::make_shared<jt::Animation>();
    m_animation->add("assets/enemy_test.png", "idle", jt::Vector2u { 16, 16 }, { 0 }, 0.5f,
        getGame()->gfx().textureManager());
    m_animation->play("idle");

    b2FixtureDef fixtureDef;
    b2CircleShape circle {};
    circle.m_radius = GP::PlayerSize().x / 2.0f;

    fixtureDef.shape = &circle;
    fixtureDef.friction = 0.0f;
    getB2Body()->CreateFixture(&fixtureDef);
}

void EnemyBase::doUpdate(const float elapsed)
{
    doAI(elapsed);

    m_animation->setPosition(getPosition()
        - jt::Vector2f { m_animation->getLocalBounds().width, m_animation->getLocalBounds().height }
            * 0.5f);
    m_animation->update(elapsed);
}

void EnemyBase::doDraw() const { m_animation->draw(getGame()->gfx().target()); }
void EnemyBase::receiveDamage(const Damage& dmg)
{
    m_hitpoints -= dmg.value;
    m_animation->flash(0.1f, jt::colors::Red);
    if (m_hitpoints <= 0.0f) {
        die();
    }
}
void EnemyBase::die()
{
    // Graphical stuff; override in subclass
    // Don't forget to call kill() in subclass
    m_state.spawnExperience(m_experience, getPosition());
    kill();
}
