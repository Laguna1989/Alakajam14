#ifndef ALAKAJAM14_SPELL_INTERFACE_HPP
#define ALAKAJAM14_SPELL_INTERFACE_HPP

#include <string>

class SpellInterface {
public:
    virtual ~SpellInterface() = default;
    virtual void trigger() = 0;
    virtual bool canTrigger() const = 0;

    virtual void update(float elapsed) = 0;

    virtual int getExperienceCost() const = 0;

    virtual void onEquip() = 0;
    virtual void onUnEquip() = 0;

    virtual std::string getName() const = 0;
};

#endif // ALAKAJAM14_SPELL_INTERFACE_HPP
