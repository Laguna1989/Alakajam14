#include "state_game.hpp"
#include "box2dwrapper/box2d_world_impl.hpp"
#include "color.hpp"
#include "enemies/enemy_crystal_mid.hpp"
#include "enemies/enemy_crystal_small.hpp"
#include "enemies/enemy_grunt.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "hud/hud.hpp"
#include "math_helper.hpp"
#include "random/random.hpp"
#include "shape.hpp"
#include "sprite.hpp"
#include "state_menu.hpp"
#include "tilemap/tileson_loader.hpp"

namespace {
void camFollowObject(jt::CamInterface& cam, jt::Vector2f const& windowSize,
    std::shared_ptr<jt::Box2DObject> obj, float elapsed)
{
    auto const objPos = obj->getPosition();
    auto const camPos = cam.getCamOffset();
    auto const dif = objPos - camPos;

    float const margin = 80.0f;
    float const moveSpeed = GP::PlayerBaseMovementSpeed() * 0.8f;
    jt::Vector2f const screenSize = windowSize / cam.getZoom();
    if (dif.x < margin) {
        cam.move(jt::Vector2f { -moveSpeed, 0.0f } * elapsed);
        if (dif.x < margin * 0.5f) {
            cam.move(jt::Vector2f { -moveSpeed, 0.0f } * elapsed);
        }
    }

    if (dif.x > screenSize.x - margin) {
        cam.move(jt::Vector2f { moveSpeed, 0.0f } * elapsed);
        if (dif.x > screenSize.x - margin * 0.5f) {
            cam.move(jt::Vector2f { moveSpeed, 0.0f } * elapsed);
        }
    }

    if (dif.y < margin) {
        cam.move(jt::Vector2f { 0.0f, -moveSpeed } * elapsed);
        if (dif.y < margin * 0.5f) {
            cam.move(jt::Vector2f { 0.0f, -moveSpeed } * elapsed);
        }
    }

    if (dif.y > screenSize.y - margin) {
        cam.move(jt::Vector2f { 0.0f, moveSpeed } * elapsed);
        if (dif.y > screenSize.y - margin * 0.5f) {
            cam.move(jt::Vector2f { 0.0f, moveSpeed } * elapsed);
        }
    }
}
} // namespace

void StateGame::doInternalCreate()
{
    m_world = std::make_shared<jt::Box2DWorldImpl>(jt::Vector2f { 0.0f, 0.0f });

    m_contactListener = std::make_shared<ShroomGameContactListener>(*this);
    m_world->setContactListener(m_contactListener);

    float const w = static_cast<float>(GP::GetWindowSize().x);
    float const h = static_cast<float>(GP::GetWindowSize().y);

    using jt::Shape;

    m_vignette = std::make_shared<jt::Sprite>("#v#"
            + std::to_string(static_cast<int>(GP::GetScreenSize().x)) + "#"
            + std::to_string(static_cast<int>(GP::GetScreenSize().y)),
        getGame()->gfx().textureManager());
    m_vignette->setIgnoreCamMovement(true);
    m_vignette->setColor({ 255, 255, 255, 100 });

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    loadTilemap();

    createPlayer();

    createEnemies();

    createExperienceOrbs();

    createSnipeProjectilesGroup();

    // StateGame will call drawObjects itself.
    setAutoDraw(false);
}
void StateGame::createSnipeProjectilesGroup()
{
    m_snipeProjectiles = std::make_shared<jt::ObjectGroup<SnipeProjectile>>();
    add(m_snipeProjectiles);
}
void StateGame::createExperienceOrbs()
{
    m_experienceOrbs = std::make_shared<jt::ObjectGroup<ExperienceOrb>>();
    add(m_experienceOrbs);
}

void StateGame::createEnemies()
{
    m_enemies = std::make_shared<jt::ObjectGroup<EnemyBase>>();

    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(3 * GP::PlayerSize().x, 7 * GP::PlayerSize().y);
    bodyDef.linearDamping = 16.0f;
    auto e = std::make_shared<EnemyCrystalSmall>(m_world, &bodyDef, *this);
    m_enemies->push_back(e);
    add(e);

    b2BodyDef bodyDef2;
    bodyDef2.fixedRotation = true;
    bodyDef2.type = b2_dynamicBody;
    bodyDef2.position.Set(5 * GP::PlayerSize().x, 9 * GP::PlayerSize().y);
    bodyDef2.linearDamping = 16.0f;
    auto e2 = std::make_shared<EnemyCrystalMid>(m_world, &bodyDef2, *this);
    m_enemies->push_back(e2);
    add(e2);
}

void StateGame::createPlayer()
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(104 * GP::PlayerSize().x, 64 * GP::PlayerSize().y);
    m_player = std::make_shared<PlayerCharacter>(m_world, &bodyDef, *this);
    add(m_player);
}

void StateGame::doInternalUpdate(float const elapsed)
{
    if (m_running) {
        m_world->step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());
        // update game logic here

        m_tileLayerGround1->update(elapsed);
        m_tileLayerOverlay->update(elapsed);

        updateTileNodes(elapsed);

        camFollowObject(
            getGame()->gfx().camera(), getGame()->gfx().window().getSize(), m_player, elapsed);

        updateExperience();
    }

    m_vignette->update(elapsed);
}

void StateGame::updateExperience() const
{
    for (auto const& e : *m_experienceOrbs) {
        auto experienceOrb = e.lock();
        if (!experienceOrb) {
            continue;
        }
        if (experienceOrb->getAge() < GP::ExperienceOrbIdleTime()) {
            continue;
        }

        auto const playerPosition = m_player->getPosition();
        auto const orbPosition = experienceOrb->getPosition();
        auto diff = playerPosition - orbPosition;
        auto const distance = jt::MathHelper::lengthSquared(diff);

        if (distance < GP::ExperienceOrbAttractDistance() * GP::ExperienceOrbAttractDistance()) {
            jt::MathHelper::normalizeMe(diff);
            experienceOrb->setVelocity(diff * GP::ExperienceOrbVelocity());
        }
        if (distance < GP::ExperienceOrbPickupDistance() * GP::ExperienceOrbPickupDistance()) {
            experienceOrb->kill();
            m_player->gainExperience(experienceOrb->m_value);
        }
    }
}
void StateGame::updateTileNodes(float const elapsed)
{
    for (auto const& t : m_nodeLayer->getAllTiles()) {

        t->getDrawable()->update(elapsed);
    }
}

void StateGame::doInternalDraw() const
{
    m_tileLayerGround1->draw(getGame()->gfx().target());
    m_tileLayerOverlay->draw(getGame()->gfx().target());
    drawObjects();
    m_experienceOrbs->draw();
    m_enemies->draw();
    //    drawTileNodeOverlay();
    m_snipeProjectiles->draw();
    m_vignette->draw(getGame()->gfx().target());
    m_hud->draw();
}
void StateGame::drawTileNodeOverlay()
{
    for (auto const& t : m_nodeLayer->getAllTiles()) {
        if (t->getBlocked()) {
            continue;
        }

        t->getDrawable()->draw(getGame()->gfx().target());
    }
}

void StateGame::endGame()
{
    if (m_hasEnded) {
        // trigger this function only once
        return;
    }
    m_hasEnded = true;
    m_running = false;

    getGame()->getStateManager().switchState(std::make_shared<StateMenu>());
}
std::string StateGame::getName() const { return "Game"; }

void StateGame::loadTilemap()
{
    jt::tilemap::TilesonLoader loader { "assets/cakeworld.json" };
    std::cout << "wat" << std::endl;
    m_tileLayerGround1 = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("ground1", getGame()->gfx().textureManager()));
    std::cout << "bat" << std::endl;
    m_tileLayerGround1->setScreenSizeHint(jt::Vector2f { 400, 300 });
    m_tileLayerOverlay = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("overlay", getGame()->gfx().textureManager()));
    std::cout << "vat" << std::endl;
    m_tileLayerOverlay->setScreenSizeHint(jt::Vector2f { 400, 300 });
    std::cout << "hat" << std::endl;
    m_nodeLayer = std::make_shared<jt::tilemap::NodeLayer>(
        loader.loadNodesFromLayer("ground1", getGame()->gfx().textureManager()));
    std::cout << "what" << std::endl;
    auto tileCollisions = loader.loadCollisionsFromLayer("ground1");
    std::cout << "bwat" << std::endl;
    auto const levelColliderCountInitial = tileCollisions.getRects().size();
    std::cout << "mwat" << std::endl;
    tileCollisions.refineColliders();
    std::cout << "wwat" << std::endl;
    auto const levelColliderCountOptimized = tileCollisions.getRects().size();
    getGame()->getLogger().debug(
        "Level colliders initial: " + std::to_string(levelColliderCountInitial)
            + " and optimized: " + std::to_string(levelColliderCountOptimized),
        { "level" });
    for (auto const& r : tileCollisions.getRects()) {
        b2BodyDef bodyDef;
        bodyDef.fixedRotation = true;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(r.left + r.width / 2.0f, r.top + r.height / 2.0f);

        b2FixtureDef fixtureDef;
        b2PolygonShape boxCollider {};
        boxCollider.SetAsBox(r.width / 2.0f, r.height / 2.0f);
        fixtureDef.shape = &boxCollider;

        auto collider = std::make_shared<jt::Box2DObject>(m_world, &bodyDef);
        collider->getB2Body()->CreateFixture(&fixtureDef);

        m_colliders.push_back(collider);
    }
}

std::shared_ptr<PlayerCharacter> StateGame::getPlayer() { return m_player; }

std::shared_ptr<jt::pathfinder::NodeInterface> StateGame::getTileAtPosition(
    jt::Vector2f const& actorPosInFloat)
{
    m_nodeLayer->reset();
    // TODO introduce TileSize in GP
    auto const actorPosInInt
        = jt::Vector2u { static_cast<unsigned int>(actorPosInFloat.x / GP::PlayerSize().x),
              static_cast<unsigned int>(actorPosInFloat.y / GP::PlayerSize().y) };

    return m_nodeLayer->getTileAt(actorPosInInt)->getNode();
}

std::shared_ptr<jt::ObjectGroup<EnemyBase>> StateGame::getEnemies() { return m_enemies; }

void StateGame::spawnExperience(int amount, jt::Vector2f const& pos)
{
    while (amount >= 5) {
        int value = jt::Random::getInt(1, 5);
        amount -= value;
        spawnOneExperienceOrb(pos, value);
    }
    spawnOneExperienceOrb(pos, amount);
}

void StateGame::spawnOneExperienceOrb(jt::Vector2f const& pos, int value)
{
    auto direction = jt::Random::getRandomPointIn(jt::Rectf { -32.0f, -32.0f, 64.0f, 64.0f });

    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.linearDamping = 1.0f;

    auto e = std::make_shared<ExperienceOrb>(m_world, &bodyDef, pos, value);
    e->setVelocity(direction);
    add(e);
    m_experienceOrbs->push_back(e);
}

void StateGame::spawnSnipeProjectile(jt::Vector2f const& position, jt::Vector2f const& velocity)
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;

    bodyDef.position.Set(position.x, position.y);

    auto projectile = std::make_shared<SnipeProjectile>(m_world, &bodyDef);
    projectile->setVelocity(velocity * GP::SpellSnipeProjectileSpeed());

    m_snipeProjectiles->push_back(projectile);
    add(projectile);
}

std::shared_ptr<jt::ObjectGroup<SnipeProjectile>> StateGame::getSnipeProjectiles() const
{
    return m_snipeProjectiles;
}

void StateGame::spawnBroadProjectile(jt::Vector2f const& position, jt::Vector2f const& velocity)
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;

    bodyDef.position.Set(position.x, position.y);

    auto projectile = std::make_shared<SnipeProjectile>(m_world, &bodyDef);
    projectile->setVelocity(velocity * GP::SpellBroadProjectileSpeed());

    m_snipeProjectiles->push_back(projectile);
    add(projectile);
}
