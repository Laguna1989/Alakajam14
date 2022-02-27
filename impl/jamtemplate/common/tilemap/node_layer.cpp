#include "node_layer.hpp"
#include "pathfinder/node.hpp"
#include <utility>

namespace jt {
namespace tilemap {

NodeLayer::NodeLayer(std::vector<std::shared_ptr<TileNode>> nodeTiles)
    : m_nodeTiles { std::move(nodeTiles) }
{
    createNodeConnections();
}

void NodeLayer::createNodeConnections()
{
    for (auto& t : m_nodeTiles) {
        if (t->getBlocked()) {
            continue;
        }

        auto const currentPos = t->getNode()->getTilePosition();
        for (int i = -1; i != 2; ++i) {
            for (int j = -1; j != 2; ++j) {
                if (i == 0 && j == 0) {
                    continue;
                }
                auto const oi = static_cast<int>(currentPos.x) + i;
                auto const oj = static_cast<int>(currentPos.y) + j;
                auto const ot = getTileAt(
                    jt::Vector2u { static_cast<unsigned int>(oi), static_cast<unsigned int>(oj) });
                if (ot) {
                    if (ot->getBlocked()) {
                        continue;
                    }
                    t->getNode()->addNeighbour(ot->getNode());
                }
            }
        }
    }
}

std::shared_ptr<TileNode> NodeLayer::getTileAt(jt::Vector2u const& pos)
{
    if (m_lookupHelper.count(pos) == 0) {
        auto it = std::find_if(m_nodeTiles.begin(), m_nodeTiles.end(), [&pos](auto tile) {
            auto nodeTilePos = tile->getNode()->getTilePosition();
            return nodeTilePos.x == pos.x && nodeTilePos.y == pos.y;
        });
        if (it == m_nodeTiles.end()) {
            return nullptr;
        }
        m_lookupHelper[pos] = *it;
    }
    return m_lookupHelper.at(pos);
}

std::vector<std::shared_ptr<TileNode>> NodeLayer::getAllTiles() { return m_nodeTiles; }

void NodeLayer::reset()
{
    for (auto& t : m_nodeTiles) {
        auto color = jt::MakeColor::FromRGBA(1, 1, 1, 100);
        if (!t->getBlocked()) {
            color = jt::MakeColor::FromRGBA(255, 255, 255, 100);
        }
        t->getDrawable()->setColor(color);
        t->reset();
    }
}

} // namespace tilemap
} // namespace jt
