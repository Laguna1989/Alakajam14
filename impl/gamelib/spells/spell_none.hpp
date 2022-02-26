#ifndef ALAKAJAM14_SPELL_NONE_HPP
#define ALAKAJAM14_SPELL_NONE_HPP

#include "spell_interface.hpp"

class SpellNone : public SpellInterface {
public:
    void trigger() override;
    bool canTrigger() const override;
    void update(float elapsed) override;
    int getExperienceCost() const override;
    void onEquip() override;
    void onUnEquip() override;
    std::string getName() const override;
};

#endif // ALAKAJAM14_SPELL_NONE_HPP
