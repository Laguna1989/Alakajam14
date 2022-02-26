#include "experience_orb.hpp"
#include "game_interface.hpp"

ExperienceOrb::ExperienceOrb(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def,
    jt::Vector2f const& pos, int value)
    : jt::Box2DObject { world, def }
{
    m_value = value;
    m_animation = std::make_shared<jt::Animation>();
    m_animation->setPosition(pos);
}

void ExperienceOrb::doCreate()
{
    m_animation->add("assets/enemy_test.png", "idle", { 16, 16 }, { 0 }, 0.2f,
        getGame()->gfx().textureManager());
    m_animation->play("idle");
    m_animation->setScale(jt::Vector2f { 0.5f, 0.5f });
}

void ExperienceOrb::doUpdate(float const elapsed)
{
    m_animation->setPosition(getPosition());
    m_animation->update(elapsed);
}

void ExperienceOrb::doDraw() const { m_animation->draw(getGame()->gfx().target()); }

void ExperienceOrb::doKill() { }
