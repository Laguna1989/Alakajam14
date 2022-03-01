#include "guile.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"

Guile::Guile(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def,
    std::weak_ptr<Player> player)
    : Box2DObject(world, def)
    , m_player { player }
{
}

void Guile::doCreate()
{
    b2FixtureDef fixtureDef;
    b2PolygonShape boxCollider {};
    boxCollider.SetAsBox(GP::PlayerSize().x / 2.1f, GP::PlayerSize().y / 2.1f);
    fixtureDef.shape = &boxCollider;
    getB2Body()->CreateFixture(&fixtureDef);

    m_animation = std::make_shared<jt::Animation>();

    m_animation->add("assets/guile-sheet.png", "idle", { 16, 16 }, { 0, 1, 2, 3, 4, 5, 6, 7 }, 0.2f,
        getGame()->gfx().textureManager());
    m_animation->play("idle");

    m_text = jt::dh::createText(getGame()->gfx().target(), m_textString, 12);
}

void Guile::doUpdate(float const elapsed)
{
    //    std::cout << "guile update\n";
    m_animation->setPosition(getPosition() - jt::Vector2f { 8.0f, 8.0f });
    m_animation->update(elapsed);
    m_text->setPosition(
        getPosition() - jt::Vector2f { 0, 32 } + jt::Vector2f { 0.0f, sin(m_age) * 5 });
    m_text->update(elapsed);
    auto player = m_player.lock();
    if (!player) {
        return;
    }
    auto const playerPos = player->getPosition();
    auto const guilePos = getPosition();

    float const distance = jt::MathHelper::lengthSquared(playerPos - guilePos);

    if (!m_hasGivenSpell) {

        if (distance < 32 * 32) {
            player->getSpellBook()->makeSpellAvailable(m_spellToGive);
            m_hasGivenSpell = true;
            getGame()->getLogger().debug("give spell: " + m_spellToGive);
        }
    }
}

void Guile::doDraw() const
{
    m_animation->draw(getGame()->gfx().target());
    m_text->draw(getGame()->gfx().target());
}
