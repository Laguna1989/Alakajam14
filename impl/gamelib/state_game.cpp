#include "state_game.hpp"
#include "box2dwrapper/box2d_world_impl.hpp"
#include "color.hpp"
#include "enemies/enemy_grunt.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "hud/hud.hpp"
#include "random/random.hpp"
#include "shape.hpp"
#include "sprite.hpp"
#include "state_menu.hpp"
#include "tilemap/tileson_loader.hpp"
#include "tweens/tween_alpha.hpp"

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

    float const w = static_cast<float>(GP::GetWindowSize().x);
    float const h = static_cast<float>(GP::GetWindowSize().y);

    using jt::Shape;
    using jt::TweenAlpha;

    m_vignette = std::make_shared<jt::Sprite>("#v#"
            + std::to_string(static_cast<int>(GP::GetScreenSize().x)) + "#"
            + std::to_string(static_cast<int>(GP::GetScreenSize().y)),
        getGame()->gfx().textureManager());
    m_vignette->setIgnoreCamMovement(true);
    m_vignette->setColor({ 255, 255, 255, 100 });

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    createItemRepository();

    loadTilemap();

    createWorldItems();

    createPlayer();

    m_enemies = std::make_shared<jt::ObjectGroup<EnemyBase>>();
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(3 * GP::PlayerSize().x, 7 * GP::PlayerSize().y);
    bodyDef.linearDamping = 16.0f;
    auto e = std::make_shared<EnemyGrunt>(m_world, &bodyDef, *this);
    m_enemies->push_back(e);
    add(e);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);
}

void StateGame::createPlayer()
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(5 * GP::PlayerSize().x, 7 * GP::PlayerSize().y);
    m_player = std::make_shared<PlayerCharacter>(m_world, &bodyDef, m_itemRepository);
    add(m_player);
}

void StateGame::doInternalUpdate(float const elapsed)
{
    if (m_running) {
        m_world->step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());
        // update game logic here

        m_tileLayerGround1->update(elapsed);
        m_tileLayerOverlay->update(elapsed);
        //        std::cout << m_nodeLayer->getAllTiles().size() << std::endl;
        for (auto const& t : m_nodeLayer->getAllTiles()) {

            t->getDrawable()->update(elapsed);
        }

        camFollowObject(
            getGame()->gfx().camera(), getGame()->gfx().window().getSize(), m_player, elapsed);

        pickupItems();

        handleItemSpawns();
    }

    m_vignette->update(elapsed);
}

void StateGame::handleItemSpawns()
{
    std::string const& itemToSpawn = m_player->getInventory()->getAndResetItemToSpawn();
    if (itemToSpawn != "") {
        // TODO make items drop near player
        auto const px = jt::Random::getInt(2, 8);
        auto const py = jt::Random::getInt(2, 8);
        jt::Vector2f const pos { px * 24.0f, py * 24.0f };
        spawnWorldItem(itemToSpawn, pos);
    }
}

void StateGame::doInternalDraw() const
{
    m_tileLayerGround1->draw(getGame()->gfx().target());
    m_tileLayerOverlay->draw(getGame()->gfx().target());
    drawObjects();
    m_enemies->draw();
    //    drawTileNodeOverlay();
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

void StateGame::createItemRepository()
{
    m_itemRepository = std::make_shared<ItemRepository>();

    m_itemRepository->loadFromJson("assets/demos/inventory/test_items.json");
}

void StateGame::loadTilemap()
{
    jt::tilemap::TilesonLoader loader { "assets/test_level.json" };

    m_tileLayerGround1 = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("ground1", getGame()->gfx().textureManager()));

    m_tileLayerGround1->setScreenSizeHint(jt::Vector2f { 400, 300 });

    m_tileLayerOverlay = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("overlay", getGame()->gfx().textureManager()));
    m_tileLayerOverlay->setScreenSizeHint(jt::Vector2f { 400, 300 });

    m_objectsLayer
        = std::make_shared<jt::tilemap::ObjectLayer>(loader.loadObjectsFromLayer("items"));

    m_nodeLayer = std::make_shared<jt::tilemap::NodeLayer>(
        loader.loadNodesFromLayer("ground1", getGame()->gfx().textureManager()));

    auto tileCollisions = loader.loadCollisionsFromLayer("ground1");
    auto const levelColliderCountInitial = tileCollisions.getRects().size();
    tileCollisions.refineColliders();
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

void StateGame::createWorldItems()
{
    m_worldItems = std::make_shared<jt::ObjectGroup<WorldItem>>();

    for (auto it : m_objectsLayer->getObjects()) {
        if (it.type == "item") {
            auto const refId = it.properties.strings.at("referenceid");
            spawnWorldItem(refId, it.position);
        }
    }
    add(m_worldItems);
}

void StateGame::pickupItems()
{
    // TODO player should be near the item to pick it up
    // TODO keyboard press instead of mouse
    if (getGame()->input().mouse()->justPressed(jt::MouseButtonCode::MBLeft)) {
        auto const mp = getGame()->input().mouse()->getMousePositionWorld();
        for (auto& item : *m_worldItems) {
            auto const bounds = item.lock()->getDrawable()->getGlobalBounds();
            bool const overlapsX = mp.x > bounds.left && mp.x < bounds.left + bounds.width;
            bool const overlapsY = mp.y > bounds.top && mp.y < bounds.top + bounds.height;
            if (overlapsX && overlapsY) {
                m_player->getInventory()->addItem(item.lock()->getRefId());

                item.lock()->kill();
            }
        }
    }
}
void StateGame::spawnWorldItem(std::string const& itemReferenceId, jt::Vector2f const& pos)
{
    auto item = m_itemRepository->createWorldItem(
        m_itemRepository->getItemReferenceFromString(itemReferenceId),
        getGame()->gfx().textureManager());
    item->getDrawable()->setPosition(pos);
    add(item);
    m_worldItems->push_back(item);
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

    return m_nodeLayer->getTileAt(actorPosInInt.x, actorPosInInt.y)->getNode();
}
