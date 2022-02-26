#ifndef ALAKAJAM14_SPELL_IMPL_HPP
#define ALAKAJAM14_SPELL_IMPL_HPP
#include "spell_interface.hpp"

class StateGame;

class SpellImpl : public SpellInterface {
public:
    explicit SpellImpl(StateGame&);

    void update(float elapsed) override;
    void trigger() override;
    bool canTrigger() const override;

protected:
    StateGame& m_state;
    float m_cooldown { -1.0f };

private:
    virtual void doTrigger() = 0;
};

#endif // ALAKAJAM14_SPELL_IMPL_HPP
