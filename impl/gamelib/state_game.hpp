#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "audio/sound.hpp"
#include "box2dwrapper/box2d_world_interface.hpp"
#include "crystal_projectile.hpp"
#include "deferred_action_interface.hpp"
#include "experience_spawner_interface.hpp"
#include "game_state.hpp"
#include "key.hpp"
#include "object_group.hpp"
#include "particle_system.hpp"
#include "player/player.hpp"
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
class Text;
} // namespace jt

class Hud;
class Stairs;
class Enemy;
class Level;
class ExperienceOrb;
class Guile;

class StateGame : public jt::GameState,
                  public ProjectileSpawnerInterface,
                  public DeferredActionInterface,
                  public ExperienceSpawnerInterface {
public:
    std::string getName() const override;

    std::shared_ptr<Player> getPlayer();

    std::shared_ptr<jt::ObjectGroup<Enemy>> getEnemies();

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

    void loadLevel(std::string const& fileName);

private:
    std::shared_ptr<Level> m_level;
    std::shared_ptr<Player> m_player;

    std::shared_ptr<jt::ObjectGroup<Enemy>> m_enemies;
    std::shared_ptr<jt::ObjectGroup<ExperienceOrb>> m_experienceOrbs;

    std::shared_ptr<jt::ObjectGroup<SnipeProjectile>> m_snipeProjectiles;
    std::shared_ptr<jt::ObjectGroup<CrystalProjectile>> m_crystalProjectiles;

    std::shared_ptr<jt::ParticleSystem<jt::Shape, 50>> m_particlesHeal;
    std::shared_ptr<jt::ParticleSystem<jt::Shape, 50>> m_particlesSnipeParticleSystem;
    std::shared_ptr<jt::ParticleSystem<jt::Shape, 150>> m_particlesAttack;
    jt::Vector2f m_particleAttackPosition;

    std::shared_ptr<jt::ObjectGroup<Guile>> m_guys;

    std::vector<std::shared_ptr<jt::Box2DObject>> m_colliders {};

    std::shared_ptr<b2ContactListener> m_contactListener;

    std::shared_ptr<jt::Sprite> m_vignette;
    std::shared_ptr<jt::Sound> m_musicIntro;
    std::shared_ptr<jt::Sound> m_musicLoop;
    std::shared_ptr<Stairs> m_stairs;
    std::shared_ptr<Enemy> m_boss;
    std::shared_ptr<Key> m_key;
    bool m_isIntroMusicPlaying { true };
    std::chrono::time_point<std::chrono::steady_clock> m_musicLoopStartTime;

    float m_timer { 0.0f };
    bool m_touchedInput { false };
    std::shared_ptr<jt::Text> m_timerText;

    bool m_running { true };
    bool m_hasEnded { false };
    jt::Vector2f m_stairsDest;

    void doInternalCreate() override;
    void doInternalUpdate(float const elapsed) override;

    void doInternalDraw() const override;

    void endGame();
    void loadTilemap(std::string const& fileName);

    void createPlayer();
    void createEnemies();
    void createExperienceOrbs();
    void spawnOneExperienceOrb(jt::Vector2f const& pos, int value);
    void createSnipeProjectilesGroup();
    void loadTileColliders();
    void loadEnemies();
    void loadSingleLoot(jt::tilemap::InfoRect const& o);
    void createCrystalProjectilesGroup();

    void setupEnemyDependencies(std::shared_ptr<Enemy> e);
    void loadGuiles();
    void loadLoots();
    void loadPlayerSpawn();
    void loadDoorObjects();
    void loadObjects();
    void createGuiles();
    void clearOldLevel();
    void createDoorObjects();
};

#endif
