#ifndef ALAKAJAM14_SHROOM_GAME_CONTACT_LISTENER_HPP
#define ALAKAJAM14_SHROOM_GAME_CONTACT_LISTENER_HPP

#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "Box2D/Dynamics/b2WorldCallbacks.h"

class StateGame;

/// Put all collision handling here.
class ShroomGameContactListener : public b2ContactListener {
public:
    ShroomGameContactListener(StateGame&);

private:
    StateGame& m_state;
    virtual void BeginContact(b2Contact*);
    void handleSnipeProjectiles(const b2Body* bodyA, const b2Body* bodyB);
};

#endif // ALAKAJAM14_SHROOM_GAME_CONTACT_LISTENER_HPP
