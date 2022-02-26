#include "shroom_game_contact_listener.hpp"
#include "state_game.hpp"

ShroomGameContactListener::ShroomGameContactListener(StateGame& mState)
    : m_state(mState)
{
}
void ShroomGameContactListener::BeginContact(b2Contact* contact)
{
    auto fixtureA = contact->GetFixtureA();
    auto fixtureB = contact->GetFixtureB();
    auto playerBody = m_state.getPlayer()->getB2Body();
    if (fixtureA->GetBody() == playerBody || fixtureB->GetBody() == playerBody) {
        std::cout << "The player collided with something." << std::endl;
    }
}
