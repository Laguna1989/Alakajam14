#ifndef ALAKAJAM14_SPELL_IMPL_HPP
#define ALAKAJAM14_SPELL_IMPL_HPP
#include "spell_interface.hpp"

class StateGame;

class SpellTriggerImpl : public SpellInterface {
public:
    explicit SpellTriggerImpl(StateGame&);

    void update(float elapsed) override;
    void trigger() override;
    bool canTrigger() const override;
    void onEquip() override;
    void onUnEquip() override;

protected:
    StateGame& m_state;
    float m_cooldown { -1.0f };

private:
    virtual void doTrigger() = 0;
};

class SpellPassiveImpl : public SpellInterface {
public:
    explicit SpellPassiveImpl(StateGame&);

    void trigger() override;
    bool canTrigger() const override;

protected:
    StateGame& m_state;

private:
    virtual void doTrigger() = 0;
};
#endif // ALAKAJAM14_SPELL_IMPL_HPP
