#ifndef ALAKAJAM14_SPELL_PASSIVE_EXP_BOOST_HPP
#define ALAKAJAM14_SPELL_PASSIVE_EXP_BOOST_HPP

#include "spell_impl.hpp"

class SpellPassiveExpBoost : public SpellPassiveImpl {
public:
    explicit SpellPassiveExpBoost(CharacterSheetImgui& sheet);
    void onEquip() override;
    void onUnEquip() override;
    std::string getName() const override;
};

#endif // ALAKAJAM14_SPELL_PASSIVE_EXP_BOOST_HPP
