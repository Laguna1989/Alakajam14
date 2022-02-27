#include "guile.hpp"
#include "game_interface.hpp"
#include "math_helper.hpp"

Guile::Guile(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def,
    std::weak_ptr<PlayerCharacter> player)
    : Box2DObject(world, def)
    , m_player { player }
{
}

void Guile::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();

    m_animation->add("assets/guile-sheet.png", "idle", { 16, 16 }, { 0, 1, 2, 3, 4, 5, 6, 7 }, 0.2f,
        getGame()->gfx().textureManager());
    m_animation->play("idle");
}

void Guile::doUpdate(float const elapsed)
{
    m_animation->setPosition(getPosition() - jt::Vector2f { 8.0f, 8.0f });
    m_animation->update(elapsed);

    auto player = m_player.lock();
    if (!player) {
        return;
    }
    auto const playerPos = player->getPosition();
    auto const guilePos = getPosition();

    float const distance = jt::MathHelper::lengthSquared(playerPos - guilePos);

    if (distance < 32) {
        player->getSpellBook()->makeSpellAvailable(m_spellToGive);
        m_hasGivenSpell = true;
    }
}

void Guile::doDraw() const { m_animation->draw(getGame()->gfx().target()); }
