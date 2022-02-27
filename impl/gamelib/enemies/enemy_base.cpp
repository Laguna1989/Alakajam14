#include "enemy_base.hpp"
#include "character.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "state_game.hpp"
#include "timer.hpp"

EnemyBase::EnemyBase(
    std::shared_ptr<jt::Box2DWorldInterface> world, const b2BodyDef* def, StateGame& state)
    : Box2DObject { world, def }
    , m_state { state }
{
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
    if (!m_isInDieAnimation) {
        // Graphical stuff; override in subclass
        // Don't forget to call kill() in subclass
        m_isInDieAnimation = true;
        m_animation->play("dead");
        auto t = std::make_shared<jt::Timer>(
            m_animation->getCurrentAnimTotalTime(),
            [this]() {
                kill();
                m_state.spawnExperience(m_experience, getPosition());
            },
            1);
        m_state.add(t);
    }
}
