#ifndef ALAKAJAM14_SPELL_BOOK_HPP
#define ALAKAJAM14_SPELL_BOOK_HPP

#include "audio/sound.hpp"
#include "audio/sound_null.hpp"
#include "game_object.hpp"
#include "spell_interface.hpp"
#include "text.hpp"
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
    void doCreate() override;

    mutable bool m_drawSpellbook { false };

    std::vector<std::shared_ptr<SpellInterface>> getEquippedSpells();
    std::vector<std::shared_ptr<jt::Text>> getEquippedSpellTexts();

private:
    std::vector<std::shared_ptr<SpellInterface>> m_spells {};
    std::vector<std::string> m_availableSpells {};
    mutable std::vector<std::shared_ptr<SpellInterface>> m_equippedSpells {};
    std::shared_ptr<jt::Sound> m_selectSound { nullptr };
    mutable bool m_newSpell { false };
    std::shared_ptr<jt::Text> m_newSpellText { nullptr };
    std::vector<std::shared_ptr<jt::Text>> m_spellTexts {};

    void drawEquippedSpells() const;
    std::vector<std::string> getEquippableSpells() const;
};

#endif // ALAKAJAM14_SPELL_BOOK_HPP
