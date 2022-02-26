#include "character.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"

PlayerCharacter::PlayerCharacter(std::shared_ptr<jt::Box2DWorldInterface> world,
    b2BodyDef const* def, std::weak_ptr<ItemRepository> repo)
    : jt::Box2DObject { world, def }
{
    m_inventory = std::make_shared<InventoryListImgui>(repo);
    m_charsheet = std::make_shared<CharacterSheetImgui>(repo);
}

void PlayerCharacter::doCreate()
{
    b2FixtureDef fixtureDef;
    b2PolygonShape boxCollider {};
    boxCollider.SetAsBox(GP::PlayerSize().x / 2.0f, GP::PlayerSize().y / 2.0f);
    fixtureDef.shape = &boxCollider;
    getB2Body()->CreateFixture(&fixtureDef);

    createAnimation();

    m_inventory->setGameInstance(getGame());
    m_charsheet->setGameInstance(getGame());
}
void PlayerCharacter::createAnimation()
{
    m_animation = std::make_shared<jt::Animation>();
    m_animation->add("assets/player.png", "idle",
        jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
            static_cast<unsigned int>(GP::PlayerSize().y) },
        { 0 }, 0.2f, getGame()->gfx().textureManager());

    m_animation->add("assets/player.png", "right",
        jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
            static_cast<unsigned int>(GP::PlayerSize().y) },
        { 1 }, 0.2f, getGame()->gfx().textureManager());

    m_animation->add("assets/player.png", "left",
        jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
            static_cast<unsigned int>(GP::PlayerSize().y) },
        { 2 }, 0.2f, getGame()->gfx().textureManager());

    m_animation->add("assets/player.png", "up",
        jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
            static_cast<unsigned int>(GP::PlayerSize().y) },
        { 3 }, 0.2f, getGame()->gfx().textureManager());

    m_animation->add("assets/player.png", "down",
        jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
            static_cast<unsigned int>(GP::PlayerSize().y) },
        { 4 }, 0.2f, getGame()->gfx().textureManager());

    m_animation->add("assets/player.png", "dash",
        jt::Vector2u { static_cast<unsigned int>(GP::PlayerSize().x),
            static_cast<unsigned int>(GP::PlayerSize().y) },
        { 4 }, 0.2f, getGame()->gfx().textureManager());

    m_animation->play("idle");
    m_animation->setPosition(jt::Vector2f { 5 * 24, 7 * 24 });
}

void PlayerCharacter::doUpdate(float const elapsed)
{
    handleInputMovement();
    updateAnimation(elapsed);

    m_dashTimer -= elapsed;
    m_dashCooldown -= elapsed;

    m_inventory->update(elapsed);
    m_charsheet->update(elapsed);
    m_charsheet->setEquippedItems(m_inventory->getEquippedItems());
}

void PlayerCharacter::updateAnimation(float const elapsed)
{
    auto const v = getVelocity();
    if (m_dashTimer > 0) {
        if (setAnimationIfNotSet("dash")) {
            m_animation->flash(0.3f);
            auto p = getPosition();
            auto vn = v;
            jt::MathHelper::normalizeMe(vn);
            setPosition(p + jt::Vector2f { vn * GP::PlayerBaseDashDistance() });
            // TODO trigger eye candy
        }
    } else {
        // no dash
        if (jt::MathHelper::lengthSquared(v) < 2) {
            setAnimationIfNotSet("idle");
        } else if (abs(v.x) > abs(v.y)) {
            if (v.x > 0) {
                setAnimationIfNotSet("right");
            } else {
                setAnimationIfNotSet("left");
            }
        } else {
            if (v.y > 0) {
                setAnimationIfNotSet("down");
            } else {
                setAnimationIfNotSet("up");
            }
        }
    }

    m_animation->setPosition(getPosition() - GP::PlayerSize() * 0.5f);
    m_animation->update(elapsed);
}

bool PlayerCharacter::setAnimationIfNotSet(std::string const& newAnimationName)
{
    std::string const& currentAnimationNAme = m_animation->getCurrentAnimationName();
    if (currentAnimationNAme != newAnimationName) {
        m_animation->play(newAnimationName);
        return true;
    }
    return false;
}

void PlayerCharacter::handleInputMovement()
{
    auto keyboard = getGame()->input().keyboard();
    setVelocity(jt::Vector2f { 0.0f, 0.0f });

    if (m_dashTimer <= 0.2f) {
        float const speed = GP::PlayerBaseMovementSpeed();

        if (keyboard->pressed(jt::KeyCode::D)) {
            addVelocity(jt::Vector2f { speed, 0.0f });
        }
        if (keyboard->pressed(jt::KeyCode::A)) {
            addVelocity(jt::Vector2f { -speed, 0.0f });
        }

        if (keyboard->pressed(jt::KeyCode::W)) {
            addVelocity(jt::Vector2f { 0.0f, -speed });
        }
        if (keyboard->pressed(jt::KeyCode::S)) {
            addVelocity(jt::Vector2f { 0.0f, speed });
        }
    }

    if (keyboard->justPressed(jt::KeyCode::LShift)) {
        handleDashInput();
    }
}
void PlayerCharacter::handleDashInput()
{
    if (m_dashCooldown >= 0) {
        return;
    }
    if (jt::MathHelper::lengthSquared(getVelocity()) > 0) {
        // dash in direction:

        // TODO move those variables to GameProperties
        // // Make them affect by stats
        m_dashTimer = GP::PlayerDashTime();
        m_dashCooldown = GP::PlayerBaseDashCooldown();
    }
}

void PlayerCharacter::doDraw() const
{
    m_animation->draw(getGame()->gfx().target());
    m_inventory->draw();
    m_charsheet->draw();
}

std::shared_ptr<InventoryInterface> PlayerCharacter::getInventory() { return m_inventory; }
std::shared_ptr<CharacterSheetImgui> PlayerCharacter::getCharSheet() { return m_charsheet; }
