#include "state_game.hpp"
#include "box2dwrapper/box2d_world_impl.hpp"
#include "color.hpp"
#include "color_helpers.hpp"
#include "drawable_helpers.hpp"
#include "experience_orb.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "guile.hpp"
#include "healing_potion.hpp"
#include "hud/hud.hpp"
#include "key.hpp"
#include "level.hpp"
#include "pathfinder/pathfinder.hpp"
#include "player/player.hpp"
#include "random/random.hpp"
#include "shape.hpp"
#include "sprite.hpp"
#include "stairs.hpp"
#include "state_menu.hpp"
#include "timer.hpp"
#include "tweens/tween_alpha.hpp"
#include "tweens/tween_color.hpp"
#include "tweens/tween_position.hpp"
#include "tweens/tween_scale.hpp"

namespace {
void camFollowObject(jt::CamInterface& cam, jt::Vector2f const& windowSize,
    std::shared_ptr<jt::Box2DObject> obj, float elapsed)
{
    auto const objPos = obj->getPosition();
    auto const camPos = cam.getCamOffset();
    auto const dif = objPos - camPos;

    float const margin = 140.0f;
    float const moveSpeed = GP::PlayerBaseMovementSpeed() * 1.0f;
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

    using jt::Shape;

    m_vignette = std::make_shared<jt::Sprite>("#v#"
            + std::to_string(static_cast<int>(GP::GetScreenSize().x)) + "#"
            + std::to_string(static_cast<int>(GP::GetScreenSize().y)),
        getGame()->gfx().textureManager());
    m_vignette->setIgnoreCamMovement(true);
    m_vignette->setColor({ 255, 255, 255, 180 });

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    createPlayer();
    createEnemies();
    createExperienceOrbs();
    createHealingPotions();
    createGuiles();
    createSnipeProjectilesGroup();
    createCrystalProjectilesGroup();
    createDoorObjects();
    loadLevel("assets/cakeworld.json");

    // StateGame will call drawObjects itself.
    setAutoDraw(false);

    m_musicIntro = std::make_shared<jt::Sound>("assets/sound/alaka2022_main_theme_v1_intro.ogg");
    m_musicIntro->play();

    m_musicLoop = std::make_shared<jt::Sound>("assets/sound/alaka2022_main_theme_v1_loop.ogg");
    m_musicLoop->setLoop(true);

    m_timerText = jt::dh::createText(getGame()->gfx().target(), "0.00", 8);
    m_timerText->setPosition(jt::Vector2f { 360.0f, 30.0f });
    m_timerText->setIgnoreCamMovement(true);
    m_timerText->setTextAlign(jt::Text::TextAlign::LEFT);
}
void StateGame::createDoorObjects()
{
    m_stairs = std::make_shared<Stairs>(*this);
    add(m_stairs);

    m_key = std::make_shared<Key>(*this);
    add(m_key);
}
void StateGame::createGuiles() { m_guys = std::make_shared<jt::ObjectGroup<Guile>>(); }
namespace {
template <typename T>
void clearGroup(std::shared_ptr<jt::ObjectGroup<T>> group)
{
    for (auto const& e : *group) {
        auto entry = e.lock();
        if (!entry) {
            continue;
        }
        entry->kill();
    }
}
} // namespace

void StateGame::loadLevel(std::string const& fileName)
{
    clearOldLevel();
    loadTilemap(fileName);
}

void StateGame::clearOldLevel()
{
    clearGroup(m_snipeProjectiles);
    clearGroup(m_crystalProjectiles);
    clearGroup(m_enemies);
    clearGroup(m_experienceOrbs);
    clearGroup(m_guys);
    for (auto c : m_colliders) {
        c->destroy();
    }
    m_colliders.clear();

    if (m_level) {
        m_level->kill();
    }
    basicUpdateObjects(0.1f);
}

void StateGame::createSnipeProjectilesGroup()
{
    m_snipeProjectiles = std::make_shared<jt::ObjectGroup<SnipeProjectile>>();
    add(m_snipeProjectiles);

    m_particlesSnipeParticleSystem = std::make_shared<jt::ParticleSystem<jt::Shape, 50>>(
        [this]() {
            auto shape = std::make_shared<jt::Shape>();
            shape->makeRect({ 1, 1 }, getGame()->gfx().textureManager());
            shape->setPosition({ -50000, -500000 });
            shape->setColor(jt::MakeColor::FromHexString("6110a2"));
            return shape;
        },
        [this](std::shared_ptr<jt::Shape> shape) {
            jt::Vector2f pos = jt::Random::getRandomPointIn(jt::Rectf {
                m_particlesSnipePosition.x - 3, m_particlesSnipePosition.y - 3, 6.0f, 6.0f });
            shape->setPosition(pos);

            auto twPos = jt::TweenPosition::create(
                shape, 0.75f, pos, pos + jt::Vector2f { 0, jt::Random::getFloat(4.0f, 12.0f) });
            add(twPos);

            auto twAlpha = jt::TweenAlpha::create(shape, 0.7f, 250, 0);
            add(twAlpha);
        });
    add(m_particlesSnipeParticleSystem);
}

void StateGame::createCrystalProjectilesGroup()
{
    m_crystalProjectiles = std::make_shared<jt::ObjectGroup<CrystalProjectile>>();
    add(m_crystalProjectiles);
}

void StateGame::createExperienceOrbs()
{
    m_experienceOrbs = std::make_shared<jt::ObjectGroup<ExperienceOrb>>();
    add(m_experienceOrbs);
}

void StateGame::createHealingPotions()
{
    m_healingPotions = std::make_shared<jt::ObjectGroup<HealingPotion>>();
    add(m_healingPotions);
}

void StateGame::createEnemies() { m_enemies = std::make_shared<jt::ObjectGroup<Enemy>>(); }

void StateGame::createPlayer()
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(104 * GP::PlayerSize().x, 64 * GP::PlayerSize().y);
    m_player = std::make_shared<Player>(m_world, &bodyDef, *this);

    m_particlesHeal = std::make_shared<jt::ParticleSystem<jt::Shape, 50>>(
        [this]() {
            auto shape = std::make_shared<jt::Shape>();
            shape->makeRect({ 1, 4 }, getGame()->gfx().textureManager());
            shape->setPosition({ -50000, -500000 });
            shape->setColor(jt::colors::Green);
            shape->setScale({ 1.0f, 0.1f });
            return shape;
        },
        [this](std::shared_ptr<jt::Shape> shape) {
            jt::Vector2f pos = jt::Random::getRandomPointIn(jt::Rectf {
                m_player->getPosition().x - 12, m_player->getPosition().y - 8.0f, 24.0f, 24.0f });
            shape->setPosition(pos);

            auto twPos = jt::TweenPosition::create(shape, 0.75f, pos, pos + jt::Vector2f { 0, -8 });
            add(twPos);

            auto twAlpha = jt::TweenAlpha::create(shape, 0.7f, 250, 0);
            add(twAlpha);

            auto twScale = jt::TweenScale::create(shape, 0.75f, { 1.0f, 0.1f }, { 0.0f, 1.5f });
            add(twScale);
        });
    add(m_particlesHeal);

    m_player->setHealCallback([this]() {
        auto t = std::make_shared<jt::Timer>(
            0.15f, [this]() { m_particlesHeal->Fire(10); }, 5);
        add(t);
    });

    m_particlesAttack = std::make_shared<jt::ParticleSystem<jt::Shape, 150>>(
        [this]() {
            auto shape = std::make_shared<jt::Shape>();
            shape->makeRect({ 1, 1 }, getGame()->gfx().textureManager());
            shape->setPosition({ -50000, -500000 });
            shape->setColor(jt::Color { 146, 65, 243 });
            shape->setScale({ 1.0f, 1.0f });
            return shape;
        },
        [this](std::shared_ptr<jt::Shape> shape) {
            jt::Vector2f initialPosition = m_particleAttackPosition;

            /*   jt::Vector2f offset
                   = jt::Random::getRandomPointIn(jt::Rectf { -12.0f, -8.0f, 24.0f, 24.0f });
              */
            auto const angle = jt::MathHelper::deg2rad(jt::Random::getFloat(0.0f, 360.f));
            auto const distance = jt::Random::getFloat(0.0f, 45.0f / 4.0f);
            jt::Vector2f offset { distance * sin(angle), distance * cos(angle) };
            shape->setPosition(initialPosition + offset * 0.3f);

            auto twPos = jt::TweenPosition::create(
                shape, 0.25f, initialPosition + offset * 0.3f, initialPosition + offset);
            add(twPos);

            auto twScale = jt::TweenScale::create(shape, 0.75f, { 1.0f, 1.0f }, { 2.0f, 2.0f });
            add(twScale);

            auto twColor = jt::TweenColor::create(
                shape, 0.25f, jt::Color { 146, 65, 243, 255 }, jt::Color { 159, 101, 207, 255 });
            add(twColor);

            auto twAlpha = jt::TweenAlpha::create(shape, 0.15f, 255, 0);
            twAlpha->setStartDelay(0.4f);
            add(twAlpha);
        });
    add(m_particlesAttack);
    m_player->setAttackCallback([this](jt::Vector2f position) {
        m_particleAttackPosition = position;
        m_particlesAttack->Fire(50);
    });
    add(m_player);
}

void StateGame::doInternalUpdate(float const elapsed)
{
    if (m_touchedInput) {
        m_timer += elapsed;
    }
    m_timerText->setText(jt::MathHelper::floatToStringWithXDigits(m_timer, 2));
    m_timerText->update(elapsed);
    if (m_running) {
        m_world->step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());

        for (auto k : jt::getAllKeys()) {
            if (getGame()->input().keyboard()->justPressed(k)) {
                m_touchedInput = true;
            }
        }
        for (auto sp : *m_snipeProjectiles) {
            auto projectile = sp.lock();
            if (!projectile) {
                continue;
            }
            m_particlesSnipePosition = projectile->getPosition();
            m_particlesSnipeParticleSystem->Fire(1);
        }

        camFollowObject(
            getGame()->gfx().camera(), getGame()->gfx().window().getSize(), m_player, elapsed);

        if (m_player->getCharSheet()->getHitpoints() < 0) {
            m_player->die();

            endGame();
        }

        if (!m_hasEnded) {

            if (m_boss) {
                if (!m_boss->isAlive()) {
                    m_hasEnded = true;
                    auto stateMenu = std::make_shared<StateMenu>();
                    if (!getGame()->wasCheating()) {
                        stateMenu->setScore(m_timer);
                    }
                    getGame()->getStateManager().switchState(stateMenu);
                }
            }
        }

        if (m_musicIntro->isPlaying()) {
            m_musicIntro->update();
        } else {
            if (m_isIntroMusicPlaying) {
                m_isIntroMusicPlaying = false;
                m_musicLoop->play();
                m_musicLoopStartTime = std::chrono::steady_clock::now();
            }

            // Yay, ugly hack to make sure looping sound is being looped
            auto const now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(now - m_musicLoopStartTime).count()
                >= 32) {
                m_musicLoopStartTime = now;
                m_musicLoop->stop();
                m_musicLoop->play();
            }
            m_musicLoop->update();
        }
    }

    m_vignette->update(elapsed);
}

void StateGame::doInternalDraw() const
{
    m_level->drawLowerLayers();

    drawObjects();
    m_experienceOrbs->draw();
    for (auto o : *m_enemies) {
        auto ob = o.lock();
        if (ob == nullptr) {
            continue;
        }
        ob->draw();
    }
    m_snipeProjectiles->draw();
    m_crystalProjectiles->draw();
    m_stairs->draw();
    m_key->draw();
    m_level->drawUpperLayers();
    m_player->drawOverlay();
    m_vignette->draw(getGame()->gfx().target());
    m_timerText->draw(getGame()->gfx().target());
    m_hud->draw();
    for (auto const e : *m_enemies) {
        auto enemy = e.lock();
        if (enemy) {
            enemy->drawHud();
        }
    }
}

void StateGame::endGame()
{
    if (m_hasEnded) {
        // trigger this function only once
        return;
    }

    if (m_player->m_hasFinishedDying) {
        m_hasEnded = true;
        m_running = false;
        getGame()->getStateManager().switchState(std::make_shared<StateMenu>());
    }
}

std::string StateGame::getName() const { return "Game"; }

void StateGame::loadTilemap(std::string const& fileName)
{
    m_level = std::make_shared<Level>(fileName);
    add(m_level);

    loadTileColliders();
    loadObjects();
}

void StateGame::loadObjects()
{
    loadEnemies();
    loadGuiles();
    loadLoots();
    loadDoorObjects();
    loadPlayerSpawn();
}
void StateGame::loadDoorObjects()
{
    m_stairs->setPosition(m_level->getStairsPosition());
    m_key->setPosition(m_level->getKeysPosition());
    m_stairsDest = m_level->getDestPosition();
}
void StateGame::loadPlayerSpawn()
{
    m_player->setPosition(m_level->getPlayerSpawn());
    getGame()->gfx().camera().setCamOffset(m_player->getPosition() - GP::GetScreenSize() / 2.0f);
}

void StateGame::loadLoots()
{
    for (auto const& l : m_level->getLootInfo()) {
        loadSingleLoot(l);
    }
}

void StateGame::loadGuiles()
{
    for (auto const& guile : m_level->createGuiles(m_world, m_player)) {
        m_guys->push_back(guile);
        add(guile);
    }
}

void StateGame::loadEnemies()
{
    auto enemies = m_level->createEnemies(m_world);
    for (auto e : enemies) {
        setupEnemyDependencies(e);
        m_enemies->push_back(e);
        add(e);
        if (e->isBoss()) {
            m_boss = e;
        }
    }
    getGame()->getLogger().debug("parsed N =" + std::to_string(m_enemies->size()) + " enemies");
}

void StateGame::loadSingleLoot(jt::tilemap::InfoRect const& o)
{
    if (o.properties.strings.at("lootType") == "xp_small") {
        spawnExperience(GP::LootExperienceSmallAmount(), o.position, true);
    } else if (o.properties.strings.at("lootType") == "xp_medium") {
        spawnExperience(GP::LootExperienceMediumAmount(), o.position, true);
    } else if (o.properties.strings.at("lootType") == "xp_large") {
        spawnExperience(GP::LootExperienceLargeAmount(), o.position, true);
    } else if (o.properties.strings.at("lootType") == "healing_potion") {
        spawnHealingPotion(o.position);
    }
}

void StateGame::setupEnemyDependencies(std::shared_ptr<Enemy> e)
{
    e->setTarget(m_player);
    e->setProjectileSpawner(this);
    e->setExperienceSpawner(this);
    e->setPathCalculator(m_level.get());
    e->setDeferredActionHandler(this);
}

void StateGame::loadTileColliders() { m_colliders = m_level->createColliders(m_world); }

std::shared_ptr<Player> StateGame::getPlayer() { return m_player; }

std::shared_ptr<jt::ObjectGroup<Enemy>> StateGame::getEnemies() { return m_enemies; }

void StateGame::spawnExperience(int amount, jt::Vector2f const& pos, bool single)
{
    if (!single) {
        while (amount >= 5) {
            int value = jt::Random::getInt(1, 5);
            amount -= value;
            spawnOneExperienceOrb(pos, value);
        }
    }
    spawnOneExperienceOrb(pos, amount);
}

void StateGame::spawnHealingPotion(jt::Vector2f const& pos)
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.linearDamping = 1.0f;

    auto e = std::make_shared<HealingPotion>(m_world, &bodyDef, m_player);
    add(e);
    m_healingPotions->push_back(e);
}

void StateGame::spawnOneExperienceOrb(jt::Vector2f const& pos, int value)
{
    auto direction = jt::Random::getRandomPointIn(jt::Rectf { -48.0f, -48.0f, 96.0f, 96.0f });

    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    bodyDef.linearDamping = 1.0f;

    auto e = std::make_shared<ExperienceOrb>(m_world, &bodyDef, value, m_player);
    e->setVelocity(direction);
    add(e);
    m_experienceOrbs->push_back(e);
}

void StateGame::spawnSnipeProjectile(
    jt::Vector2f const& position, jt::Vector2f const& velocity, Damage const& damage)
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;

    bodyDef.position.Set(position.x, position.y);

    auto projectile = std::make_shared<SnipeProjectile>(m_world, &bodyDef);
    projectile->setVelocity(velocity);
    projectile->setRotation(atan2(-velocity.y, velocity.x) * 180 / 3.1415f);
    projectile->setDamage(damage);

    m_snipeProjectiles->push_back(projectile);
    add(projectile);
}

void StateGame::spawnCrystalProjectile(
    jt::Vector2f const& position, jt::Vector2f const& velocity, bool isBoss)
{
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;

    bodyDef.position.Set(position.x, position.y);

    auto projectile = std::make_shared<CrystalProjectile>(m_world, &bodyDef, isBoss);
    projectile->setVelocity(velocity);

    m_crystalProjectiles->push_back(projectile);
    add(projectile);
}

void StateGame::queueDeferredAction(float time, std::function<void(void)> const& action)
{
    auto const t = std::make_shared<jt::Timer>(time, action, 1);
    add(t);
}

std::shared_ptr<jt::ObjectGroup<SnipeProjectile>> StateGame::getSnipeProjectiles() const
{
    return m_snipeProjectiles;
}

std::shared_ptr<jt::ObjectGroup<CrystalProjectile>> StateGame::getCrystalProjectiles() const
{
    return m_crystalProjectiles;
}

std::shared_ptr<Stairs> StateGame::getStairs() const { return m_stairs; }

jt::Vector2f& StateGame::getStairsDest() { return m_stairsDest; }
