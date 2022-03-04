#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "audio/sound.hpp"
#include "box2dwrapper/box2d_world_interface.hpp"
#include "crystal_projectile.hpp"
#include "deferred_action_interface.hpp"
#include "experience_orb.hpp"
#include "experience_spawner_interface.hpp"
#include "game_state.hpp"
#include "guile.hpp"
#include "key.hpp"
#include "object_group.hpp"
#include "player.hpp"
#include "projectile_spawner_interface.hpp"
#include "shroom_game_contact_listener.hpp"
#include "snipe_projectile.hpp"
#include "tilemap/node_layer.hpp"
#include "tilemap/object_layer.hpp"
#include "tilemap/tile_layer.hpp"
#include "tilemap/tileson_loader.hpp"
#include <chrono>
#include <memory>
#include <string>
#include <vector>

// fwd decls
namespace jt {
class Sprite;
} // namespace jt

class Hud;
class Stairs;
class EnemyBase;
class Level;

class StateGame : public jt::GameState,
                  public ProjectileSpawnerInterface,
                  public DeferredActionInterface,
                  public ExperienceSpawnerInterface {
public:
    std::string getName() const override;

    std::shared_ptr<Player> getPlayer();

    std::shared_ptr<jt::ObjectGroup<EnemyBase>> getEnemies();

    void spawnExperience(int amount, jt::Vector2f const& pos, bool single) override;

    std::shared_ptr<Hud> m_hud;

    void spawnSnipeProjectile(jt::Vector2f const& position, jt::Vector2f const& velocity);
    void spawnBroadProjectile(jt::Vector2f const& position, jt::Vector2f const& velocity);

    void spawnCrystalProjectile(
        jt::Vector2f const& position, jt::Vector2f const& velocity, bool isBoss) override;

    void queueDeferredAction(float time, std::function<void(void)> const& action) override;

    std::shared_ptr<jt::Box2DWorldInterface> m_world { nullptr };
    jt::Vector2f m_particlesSnipePosition { 0.0f, 0.0f };

    std::shared_ptr<jt::ObjectGroup<SnipeProjectile>> getSnipeProjectiles() const;
    std::shared_ptr<jt::ObjectGroup<CrystalProjectile>> getCrystalProjectiles() const;

    std::shared_ptr<Stairs> getStairs() const;
    jt::Vector2f& getStairsDest();

private:
    std::shared_ptr<Level> m_level;

    std::shared_ptr<Player> m_player;

    std::shared_ptr<jt::ObjectGroup<EnemyBase>> m_enemies;
    std::shared_ptr<jt::ObjectGroup<ExperienceOrb>> m_experienceOrbs;

    std::shared_ptr<jt::ObjectGroup<SnipeProjectile>> m_snipeProjectiles;
    std::shared_ptr<jt::ObjectGroup<CrystalProjectile>> m_crystalProjectiles;

    std::shared_ptr<jt::ParticleSystem<jt::Shape, 50>> m_particlesHeal;
    std::shared_ptr<jt::ParticleSystem<jt::Shape, 50>> m_particlesSnipeProjectiles;

    std::shared_ptr<jt::ObjectGroup<Guile>> m_guys;

    std::shared_ptr<jt::Sprite> m_vignette;

    std::vector<std::shared_ptr<jt::Box2DObject>> m_colliders {};

    std::shared_ptr<b2ContactListener> m_contactListener;

    std::shared_ptr<jt::Sound> m_musicIntro;
    std::shared_ptr<jt::Sound> m_musicLoop;
    std::shared_ptr<Stairs> m_stairs;
    std::shared_ptr<Key> m_key;
    bool m_isIntroMusicPlaying { true };
    std::chrono::time_point<std::chrono::steady_clock> m_musicLoopStartTime;

    bool m_running { true };

    bool m_hasEnded { false };

    void doInternalCreate() override;
    void doInternalUpdate(float const elapsed) override;

    void doInternalDraw() const override;

    void endGame();
    void loadTilemap();

    void createPlayer();
    void createEnemies();
    void createExperienceOrbs();
    void spawnOneExperienceOrb(jt::Vector2f const& pos, int value);
    void createSnipeProjectilesGroup();
    void loadTileColliders();
    void loadEnemies();
    void loadSingleEnemy(jt::tilemap::InfoRect const& info);
    void loadSingleEnemySmallCrystal(jt::Vector2f const& position);
    void loadSingleEnemyMediumCrystal(jt::Vector2f const& position);
    void loadSingleEnemyLargeCrystal(jt::Vector2f const& position);
    void loadSingleLoot(jt::tilemap::InfoRect const& o);
    void createCrystalProjectilesGroup();
    void loadStairs(jt::Vector2f f);
    void loadKey(jt::Vector2f f);
    jt::Vector2f m_stairsDest;
    void loadSingleEnemyBoss(const jt::Vector2f& position);
    void setupEnemyDependencies(std::shared_ptr<EnemyBase> e);
    void loadGuiles();
    void loadLoots();
    void loadPlayerSpawn();
    void loadDoorObjects();
    void loadObjects();
};

#endif
