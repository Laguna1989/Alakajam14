#include "shroom_game_contact_listener.hpp"
#include "state_game.hpp"
#include "timer.hpp"

ShroomGameContactListener::ShroomGameContactListener(StateGame& mState)
    : m_state(mState)
{
}
void ShroomGameContactListener::BeginContact(b2Contact* contact)
{
    auto bodyA = contact->GetFixtureA()->GetBody();
    auto bodyB = contact->GetFixtureB()->GetBody();

    /* auto playerBody = m_state.getPlayer()->getB2Body();
     if (bodyA == playerBody || bodyB == playerBody) {
         std::cout << "The player collided with something." << std::endl;
     }*/

    handleSnipeProjectiles(bodyA, bodyB);
}
void ShroomGameContactListener::handleSnipeProjectiles(const b2Body* bodyA, const b2Body* bodyB)
{
    for (auto sn : *m_state.getSnipeProjectiles()) {
        auto snipeProj = sn.lock();
        if (snipeProj == nullptr) {
            continue;
        }
        auto snipeBody = snipeProj->getB2Body();
        if (bodyA == snipeBody || bodyB == snipeBody) {
            for (auto nme : *m_state.getEnemies()) {
                auto enemy = nme.lock();
                if (enemy == nullptr) {
                    continue;
                }
                auto enemyBody = enemy->getB2Body();
                if (bodyA == enemyBody || bodyB == enemyBody) {
                    auto deferredAction = std::make_shared<jt::Timer>(
                        0.0f,
                        [enemy, snipeProj]() {
                            enemy->receiveDamage(snipeProj->getDamage());
                            snipeProj->kill();
                        },
                        1);
                    m_state.add(deferredAction);
                }
            }
        }
    }
}
