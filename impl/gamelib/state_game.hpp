#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "box2dwrapper/box2d_world_interface.hpp"
#include "character.hpp"
#include "enemies/enemy_base.hpp"
#include "experience_orb.hpp"
#include "game_state.hpp"
#include "object_group.hpp"
#include "shroom_game_contact_listener.hpp"
#include "snipe_projectile.hpp"
#include "tilemap/node_layer.hpp"
#include "tilemap/object_layer.hpp"
#include "tilemap/tile_layer.hpp"
#include "tilemap/tileson_loader.hpp"
#include <memory>
#include <string>
#include <vector>

// fwd decls
namespace jt {
class Sprite;
} // namespace jt

class Hud;

class StateGame : public jt::GameState {
public:
    std::string getName() const override;

    std::shared_ptr<PlayerCharacter> getPlayer();

    std::shared_ptr<jt::pathfinder::NodeInterface> getTileAtPosition(
        jt::Vector2f const& actorPosInFloat);

    std::shared_ptr<jt::ObjectGroup<EnemyBase>> getEnemies();

    void spawnExperience(int amount, jt::Vector2f const& pos);

    std::shared_ptr<Hud> m_hud;

    void spawnSnipeProjectile(jt::Vector2f const& position, jt::Vector2f const& velocity);
    void spawnBroadProjectile(jt::Vector2f const& position, jt::Vector2f const& velocity);

    std::shared_ptr<jt::Box2DWorldInterface> m_world { nullptr };

    std::shared_ptr<jt::ObjectGroup<SnipeProjectile>> getSnipeProjectiles() const;

private:
    std::shared_ptr<jt::tilemap::TileLayer> m_tileLayerGround1;
    std::shared_ptr<jt::tilemap::TileLayer> m_tileLayerOverlay;

    std::shared_ptr<jt::tilemap::NodeLayer> m_nodeLayer;

    std::shared_ptr<PlayerCharacter> m_player;

    std::shared_ptr<jt::ObjectGroup<EnemyBase>> m_enemies;
    std::shared_ptr<jt::ObjectGroup<ExperienceOrb>> m_experienceOrbs;

    std::shared_ptr<jt::ObjectGroup<SnipeProjectile>> m_snipeProjectiles;

    std::shared_ptr<jt::Sprite> m_vignette;

    std::vector<std::shared_ptr<jt::Box2DObject>> m_colliders {};

    std::shared_ptr<b2ContactListener> m_contactListener;

    bool m_running { true };

    bool m_hasEnded { false };

    void doInternalCreate() override;
    void doInternalUpdate(float const elapsed) override;

    void doInternalDraw() const override;

    void endGame();
    void loadTilemap();

    void createPlayer();
    void drawTileNodeOverlay();
    void createEnemies();
    void createExperienceOrbs();
    void spawnOneExperienceOrb(jt::Vector2f const& pos, int value);
    void updateTileNodes(float const elapsed);
    void updateExperience() const;
    void createSnipeProjectilesGroup();
    void loadTileColliders(jt::tilemap::TilesonLoader& loader);
    void loadPlayerSpawn(std::vector<jt::tilemap::InfoRect>& objects);
    void loadObjects(jt::tilemap::TilesonLoader& loader);
    void loadEnemies(std::vector<jt::tilemap::InfoRect>& objects);
    void loadSingleEnemy(jt::tilemap::InfoRect const& info);
    void loadSingleEnemySmallCrystal(jt::Vector2f const& position);
    void loadSingleEnemyMediumCrystal(jt::Vector2f const& position);
    void loadSingleEnemyLargeCrystal(jt::Vector2f const& position);
};

#endif
