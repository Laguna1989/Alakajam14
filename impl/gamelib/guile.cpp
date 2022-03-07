#include "guile.hpp"
#include "dialog/dialog.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"

Guile::Guile(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def,
    std::weak_ptr<TargetInterface> player)
    : Box2DObject(world, def)
    , m_target { player }
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

    m_dialog->setGameInstance(getGame());
    m_dialog->create();
    m_dialog->setGiveSpellCallback([this](std::string const& spell) {
        m_target.lock()->makeSpellAvailable(spell);
        getGame()->getLogger().debug("give spell: " + spell);
    });
}

void Guile::doUpdate(float const elapsed)
{
    m_animation->setPosition(getPosition() - jt::Vector2f { 8.0f, 8.0f });
    m_animation->update(elapsed);
    auto player = m_target.lock();
    if (!player) {
        return;
    }
    auto const playerPos = player->getTargetPosition();
    auto const guilePos = getPosition();

    float const distance = jt::MathHelper::lengthSquared(playerPos - guilePos);
}

void Guile::doDraw() const
{
    m_animation->draw(getGame()->gfx().target());

    auto const playerPos = m_target.lock()->getTargetPosition();

    auto const guilePos = getPosition();

    float const distance = jt::MathHelper::lengthSquared(playerPos - guilePos);

    if (distance < 32 * 32) {
        if (m_dialog) {
            m_dialog->draw();
        }
    }
}

void Guile::setDialog(DialogInfo const& info) { m_dialog = std::make_shared<Dialog>(info); }
