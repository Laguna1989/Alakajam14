#ifndef ALAKAJAM14_SPELL_HEAL_HPP
#define ALAKAJAM14_SPELL_HEAL_HPP

#include "character_sheet_imgui.hpp"
#include "spell_interface.hpp"

class SpellHeal : public SpellInterface {
public:
    SpellHeal(CharacterSheetImgui& charSheet);
    void trigger() override;
    bool canTrigger() const override;
    void update(float elapsed) override;
    int getExperienceCost() const override;
    void onEquip() override;
    void onUnEquip() override;
    std::string getName() const override;
    float m_cooldown { 0.0f };

private:
    CharacterSheetImgui& m_charSheet;
};

#endif // ALAKAJAM14_SPELL_HEAL_HPP
