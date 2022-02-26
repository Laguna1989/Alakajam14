#ifndef ALAKAJAM14_SPELL_BOOK_HPP
#define ALAKAJAM14_SPELL_BOOK_HPP

#include "spell_interface.hpp"
#include <memory>
#include <string>
#include <vector>

class StateGame;

class SpellBook {
public:
    explicit SpellBook(StateGame& state);

    std::shared_ptr<SpellInterface> getSpellByName(std::string const& name) const;
    std::vector<std::string> getAllSpellNames() const;

    std::vector<std::string> getAvailableSpellNames() const;

    void makeSpellAvailable(std::string const& name);

private:
    std::vector<std::shared_ptr<SpellInterface>> m_spells {};
    std::vector<std::string> m_availableSpells {};
};

#endif // ALAKAJAM14_SPELL_BOOK_HPP
