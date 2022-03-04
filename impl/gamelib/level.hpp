#ifndef ALAKAJAM14_LEVEL_HPP
#define ALAKAJAM14_LEVEL_HPP

#include "game_object.hpp"
#include "tilemap/info_rect.hpp"
#include "tilemap/tilemap_collisions.hpp"
#include "vector.hpp"
#include <memory>
#include <string>

namespace jt {
namespace tilemap {

class TileLayer;
class NodeLayer;

} // namespace tilemap

namespace pathfinder {
class NodeInterface;
}
} // namespace jt

class Level : public jt::GameObject {
public:
    explicit Level(std::string const& fileName);

    void doCreate() override;
    void doUpdate(float elapsed) override;

    void drawLowerLayers();
    void drawUpperLayers();

    std::shared_ptr<jt::pathfinder::NodeInterface> getTileAtPosition(
        jt::Vector2f const& actorPosInFloat);

    void drawTileNodeOverlay();
    void updateTileNodes(float const elapsed);

    std::vector<jt::Rectf> getColliderRects();
    jt::Vector2f getPlayerSpawn() const;
    // TODO split into enemy/loot/guile
    std::vector<jt::tilemap::InfoRect> getInfoRects();
    std::vector<jt::tilemap::InfoRect> getGuilesInfo();
    std::vector<jt::tilemap::InfoRect> getEnemiesInfo();
    std::vector<jt::tilemap::InfoRect> getLootInfo();

    jt::Vector2f getKeysPosition() const;
    jt::Vector2f getStairsPosition() const;
    jt::Vector2f getDestPosition() const;

    std::shared_ptr<jt::tilemap::TileLayer> m_tileLayerGround1;
    std::shared_ptr<jt::tilemap::TileLayer> m_tileLayerOverlay;
    std::shared_ptr<jt::tilemap::TileLayer> m_tileLayerOveroverlay;
    std::shared_ptr<jt::tilemap::TileLayer> m_tileLayerUnderlay;
    std::shared_ptr<jt::tilemap::TileLayer> m_tileLayerUnderunderlay;
    std::shared_ptr<jt::tilemap::NodeLayer> m_nodeLayer;

private:
    std::string m_fileName {};
    jt::TilemapCollisions m_tileCollisions;

    jt::Vector2f m_playerSpawn;
    void loadObjectPositions(std::vector<jt::tilemap::InfoRect>& objects);
    std::vector<jt::tilemap::InfoRect> m_objects;
    jt::Vector2f m_stairsPosition;
    jt::Vector2f m_keyPosition;
    jt::Vector2f m_stairsDest;
};

#endif // ALAKAJAM14_LEVEL_HPP
