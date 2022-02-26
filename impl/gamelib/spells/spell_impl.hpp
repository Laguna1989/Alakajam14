#ifndef ALAKAJAM14_SPELL_IMPL_HPP
#define ALAKAJAM14_SPELL_IMPL_HPP
#include "character_sheet_imgui.hpp"
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

class CharacterSheetImgui;
class SpellPassiveImpl : public SpellInterface {
public:
    explicit SpellPassiveImpl(CharacterSheetImgui& charSheet);

    void trigger() override;
    bool canTrigger() const override;
    int getExperienceCost() const override;
    void update(float elapsed) override;

protected:
    CharacterSheetImgui& m_charSheet;
};
#endif // ALAKAJAM14_SPELL_IMPL_HPP
