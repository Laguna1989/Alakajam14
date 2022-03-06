#include "level.hpp"
#include "enemies/enemy_crystal_boss.hpp"
#include "enemies/enemy_crystal_large.hpp"
#include "enemies/enemy_crystal_medium.hpp"
#include "enemies/enemy_crystal_small.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "pathfinder/pathfinder.hpp"
#include "strutils.hpp"
#include "tilemap/node_layer.hpp"
#include "tilemap/tile_layer.hpp"
#include "tilemap/tile_node.hpp"
#include "tilemap/tileson_loader.hpp"

Level::Level(std::string const& fileName)
    : m_fileName { fileName }
{
}

void Level::doCreate()
{
    jt::tilemap::TilesonLoader loader { m_fileName };
    m_tileLayerGround1 = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("ground1", getGame()->gfx().textureManager()));
    m_tileLayerGround1->setScreenSizeHint(jt::Vector2f { 400, 300 });
    m_tileLayerOverlay = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("overlay", getGame()->gfx().textureManager()));
    m_tileLayerOverlay->setScreenSizeHint(jt::Vector2f { 400, 300 });
    m_tileLayerUnderlay = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("underlay", getGame()->gfx().textureManager()));
    m_tileLayerUnderlay->setScreenSizeHint(jt::Vector2f { 400, 300 });
    m_tileLayerUnderunderlay = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("underunderlay", getGame()->gfx().textureManager()));
    m_tileLayerUnderunderlay->setScreenSizeHint(jt::Vector2f { 400, 300 });
    m_tileLayerOveroverlay = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("overoverlay", getGame()->gfx().textureManager()));
    m_tileLayerOveroverlay->setScreenSizeHint(jt::Vector2f { 400, 300 });
    m_nodeLayer = std::make_shared<jt::tilemap::NodeLayer>(
        loader.loadNodesFromLayer("ground1", getGame()->gfx().textureManager()));

    m_tileCollisions = loader.loadCollisionsFromLayer("ground1");
    auto const levelColliderCountInitial = m_tileCollisions.getRects().size();
    m_tileCollisions.refineColliders(16.0f);
    auto const levelColliderCountOptimized = m_tileCollisions.getRects().size();
    getGame()->getLogger().debug(
        "Level colliders initial: " + std::to_string(levelColliderCountInitial)
            + " and optimized: " + std::to_string(levelColliderCountOptimized),
        { "level" });

    m_objects = loader.loadObjectsFromLayer("objects");
    loadObjectPositions(m_objects);
}

void Level::loadObjectPositions(std::vector<jt::tilemap::InfoRect>& objects)
{
    for (auto const& o : objects) {
        if (o.name == "player_spawn") {
            m_playerSpawn = o.position;
        } else if (o.type == "stairs") {
            m_stairsPosition = o.position;
        } else if (o.type == "key") {
            m_keyPosition = o.position;
        } else if (o.type == "dest") {
            m_stairsDest = o.position;
        }
    }
}

void Level::doUpdate(float elapsed)
{
    m_tileLayerUnderlay->update(elapsed);
    m_tileLayerUnderunderlay->update(elapsed);
    m_tileLayerGround1->update(elapsed);
    m_tileLayerOverlay->update(elapsed);
    m_tileLayerOveroverlay->update(elapsed);
}

void Level::drawLowerLayers()
{
    m_tileLayerUnderunderlay->draw(getGame()->gfx().target());
    m_tileLayerUnderlay->draw(getGame()->gfx().target());
    m_tileLayerGround1->draw(getGame()->gfx().target());
    m_tileLayerOverlay->draw(getGame()->gfx().target());
}
void Level::drawUpperLayers() { m_tileLayerOveroverlay->draw(getGame()->gfx().target()); }

std::shared_ptr<jt::pathfinder::NodeInterface> Level::getTileAtPosition(
    jt::Vector2f const& actorPosInFloat)
{
    m_nodeLayer->reset();
    auto const actorPosInInt
        = jt::Vector2u { static_cast<unsigned int>(actorPosInFloat.x / GP::TileSizeInPixel()),
              static_cast<unsigned int>(actorPosInFloat.y / GP::TileSizeInPixel()) };

    return m_nodeLayer->getTileAt(actorPosInInt)->getNode();
}

void Level::drawTileNodeOverlay()
{
    for (auto const& t : m_nodeLayer->getAllTiles()) {
        if (t->getBlocked()) {
            continue;
        }

        t->getDrawable()->draw(getGame()->gfx().target());
    }
}
void Level::updateTileNodes(float const elapsed)
{
    for (auto const& t : m_nodeLayer->getAllTiles()) {

        t->getDrawable()->update(elapsed);
    }
}

jt::Vector2f Level::getPlayerSpawn() const { return m_playerSpawn; }
std::vector<jt::tilemap::InfoRect> Level::getInfoRects() { return m_objects; }

std::vector<jt::tilemap::InfoRect> Level::getGuilesInfo()
{
    std::vector<jt::tilemap::InfoRect> guileInfo;
    for (auto const& o : m_objects) {
        if (o.type == "guile") {
            guileInfo.push_back(o);
        }
    }
    return guileInfo;
}
std::vector<jt::tilemap::InfoRect> Level::getEnemiesInfo()
{
    std::vector<jt::tilemap::InfoRect> enemiesInfo;
    for (auto const& o : m_objects) {
        if (strutil::contains(o.name, "enemy")) {
            enemiesInfo.push_back(o);
        }
    }
    return enemiesInfo;
}

std::vector<jt::tilemap::InfoRect> Level::getLootInfo()
{
    std::vector<jt::tilemap::InfoRect> lootInfo;
    for (auto const& o : m_objects) {
        if (strutil::contains(o.name, "loot")) {
            lootInfo.push_back(o);
        }
    }
    return lootInfo;
}

jt::Vector2f Level::getKeysPosition() const { return m_keyPosition; }
jt::Vector2f Level::getStairsPosition() const { return m_stairsPosition; }
jt::Vector2f Level::getDestPosition() const { return m_stairsDest; }

std::vector<std::shared_ptr<jt::pathfinder::NodeInterface>> Level::calculatePath(
    jt::Vector2f const& startPos, jt::Vector2f const& endPos)
{
    auto const tileForStart = getTileAtPosition(startPos);
    auto const tileForEnd = getTileAtPosition(endPos);
    return jt::pathfinder::calculatePath(tileForStart, tileForEnd);
}
std::vector<std::shared_ptr<jt::Box2DObject>> Level::createColliders(
    std::shared_ptr<jt::Box2DWorldInterface> world)
{
    std::vector<std::shared_ptr<jt::Box2DObject>> colliders;

    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_staticBody;

    b2FixtureDef fixtureDef;
    fixtureDef.filter.categoryBits = GP::PhysicsCollisionCategoryWalls();
    fixtureDef.filter.maskBits = GP::PhysicsCollisionCategoryPlayer()
        | GP::PhysicsCollisionCategoryPlayerShots() | GP::PhysicsCollisionCategoryEnemies()
        | GP::PhysicsCollisionCategoryEnemyShots() | GP::PhysicsCollisionCategoryExperienceOrbs();

    for (auto const& r : m_tileCollisions.getRects()) {

        bodyDef.position.Set(r.left + r.width / 2.0f, r.top + r.height / 2.0f);
        b2PolygonShape boxCollider {};
        boxCollider.SetAsBox(r.width / 2.0f, r.height / 2.0f);
        fixtureDef.shape = &boxCollider;

        auto collider = std::make_shared<jt::Box2DObject>(world, &bodyDef);
        collider->getB2Body()->CreateFixture(&fixtureDef);

        colliders.push_back(collider);
    }
    return colliders;
}

b2BodyDef getEnemyDefinition(jt::Vector2f const& position)
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.linearDamping = 16.0f;
    return bodyDef;
}

std::shared_ptr<EnemyBase> loadSingleEnemySmallCrystal(
    jt::Vector2f const& position, std::shared_ptr<jt::Box2DWorldInterface> world)
{
    b2BodyDef bodyDef = getEnemyDefinition(position);
    return std::make_shared<EnemyCrystalSmall>(world, &bodyDef);
}

std::shared_ptr<EnemyBase> loadSingleEnemyMediumCrystal(
    jt::Vector2f const& position, std::shared_ptr<jt::Box2DWorldInterface> world)
{
    b2BodyDef bodyDef = getEnemyDefinition(position);
    return std::make_shared<EnemyCrystalMedium>(world, &bodyDef);
}

std::shared_ptr<EnemyBase> loadSingleEnemyLargeCrystal(
    jt::Vector2f const& position, std::shared_ptr<jt::Box2DWorldInterface> world)
{
    b2BodyDef bodyDef = getEnemyDefinition(position);
    return std::make_shared<EnemyCrystalLarge>(world, &bodyDef);
}

std::shared_ptr<EnemyBase> loadSingleEnemyBoss(
    jt::Vector2f const& position, std::shared_ptr<jt::Box2DWorldInterface> world)
{
    b2BodyDef bodyDef = getEnemyDefinition(position);
    return std::make_shared<EnemyCrystalBoss>(world, &bodyDef);
}

std::shared_ptr<EnemyBase> loadSingleEnemy(
    jt::tilemap::InfoRect const& info, std::shared_ptr<jt::Box2DWorldInterface> world)
{
    auto const position = info.position;
    auto const type = info.properties.strings.at("enemyType");
    if (type == "crystal_small") {
        return loadSingleEnemySmallCrystal(position, world);
    }
    if (type == "crystal_medium") {
        return loadSingleEnemyMediumCrystal(position, world);
    }
    if (type == "crystal_large") {
        return loadSingleEnemyLargeCrystal(position, world);
    }
    if (type == "boss") {
        return loadSingleEnemyBoss(position, world);
    }
    throw std::invalid_argument { "Enemy type " + type + " not supported" };
}

std::vector<std::shared_ptr<EnemyBase>> Level::createEnemies(
    std::shared_ptr<jt::Box2DWorldInterface> world)
{
    std::vector<std::shared_ptr<EnemyBase>> enemies {};
    for (auto const& enemyInfo : getEnemiesInfo()) {
        enemies.emplace_back(loadSingleEnemy(enemyInfo, world));
    }
    return enemies;
}
