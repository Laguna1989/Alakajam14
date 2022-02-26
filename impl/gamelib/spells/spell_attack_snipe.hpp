#ifndef ALAKAJAM14_SPELL_ATTACK_SNIPE_HPP
#define ALAKAJAM14_SPELL_ATTACK_SNIPE_HPP

class StateGame;

class SpellInterface {
public:
    virtual ~SpellInterface() = default;
    virtual void trigger() = 0;
    virtual void update(float elapsed) = 0;

    virtual int getExperienceCost() const = 0;
};

class SpellImpl : public SpellInterface {
public:
    explicit SpellImpl(StateGame&);

    void update(float elapsed) override;
    void trigger() override;

private:
    StateGame& m_state;
    float m_cooldown { -1.0f };

    virtual void doTrigger() = 0;
};

class SpellAttackSnipe : public SpellImpl {
public:
    explicit SpellAttackSnipe(StateGame&);
    void doTrigger() override;
    int getExperienceCost() const override;
};

#endif // ALAKAJAM14_SPELL_ATTACK_SNIPE_HPP
