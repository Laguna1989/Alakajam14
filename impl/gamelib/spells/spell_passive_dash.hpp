#ifndef ALAKAJAM14_SPELL_PASSIVE_DASH_HPP
#define ALAKAJAM14_SPELL_PASSIVE_DASH_HPP

#include "spell_impl.hpp"
class SpellPassiveDash : public SpellPassiveImpl {
public:
    explicit SpellPassiveDash(CharacterSheetImgui& sheet);

    void onEquip() override;
    void onUnEquip() override;

    std::string getName() const override;
};

#endif // ALAKAJAM14_SPELL_PASSIVE_DASH_HPP
