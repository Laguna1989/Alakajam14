#ifndef ALAKAJAM14_SPELL_BOOK_HPP
#define ALAKAJAM14_SPELL_BOOK_HPP

#include "game_object.hpp"
#include "spell_interface.hpp"
#include <memory>
#include <string>
#include <vector>

class StateGame;

class SpellBook : public jt::GameObject {
public:
    explicit SpellBook(StateGame& state);

    std::shared_ptr<SpellInterface> getSpellByName(std::string const& name) const;
    std::vector<std::string> getAllSpellNames() const;

    std::vector<std::string> getAvailableSpellNames() const;

    void makeSpellAvailable(std::string const& name);

    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;

    mutable bool m_drawSpellbook { false };

    std::vector<std::shared_ptr<SpellInterface>> getEquippedSpells();

private:
    std::vector<std::shared_ptr<SpellInterface>> m_spells {};
    std::vector<std::string> m_availableSpells {};
    void drawEquippedSpells() const;

    mutable std::vector<std::shared_ptr<SpellInterface>> m_equippedSpells;
    std::vector<std::string> getEquippableSpells() const;
};

#endif // ALAKAJAM14_SPELL_BOOK_HPP
