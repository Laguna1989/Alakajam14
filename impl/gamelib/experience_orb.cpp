#include "experience_orb.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "random/random.hpp"

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
    m_animation->add("assets/shards_red.png", "idle1", { 16, 16 }, { 0, 1, 2, 3 }, 0.2f,
        getGame()->gfx().textureManager());
    m_animation->add("assets/shards_green.png", "idle2", { 16, 16 }, { 0, 1, 2, 3 }, 0.2f,
        getGame()->gfx().textureManager());
    m_animation->add("assets/shards_blue.png", "idle3", { 16, 16 }, { 0, 1, 2, 3 }, 0.2f,
        getGame()->gfx().textureManager());

    std::string animName = "idle" + std::to_string(jt::Random::getInt(1, 3));
    m_animation->play(animName);
    m_animation->setScreenSizeHint(GP::GetScreenSize());
}

void ExperienceOrb::doUpdate(float const elapsed)
{
    m_animation->setPosition(getPosition() - GP::PlayerSize() * 0.5f);
    m_animation->update(elapsed);
}

void ExperienceOrb::doDraw() const { m_animation->draw(getGame()->gfx().target()); }

void ExperienceOrb::doKill() { }
