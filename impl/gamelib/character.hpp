#ifndef GUARD_JAMTEMPLATE_CHARACTER_HPP
#define GUARD_JAMTEMPLATE_CHARACTER_HPP

#include "animation.hpp"
#include "box2dwrapper/box2d_object.hpp"
#include "character_sheet_imgui.hpp"
#include "game_object.hpp"
#include "inventory/inventory_list_imgui.hpp"
#include "spells/spell_attack_snipe.hpp"
#include <memory>

class CharacterInterface {
public:
    virtual std::shared_ptr<InventoryInterface> getInventory() = 0;
    virtual std::shared_ptr<CharacterSheetImgui> getCharSheet() = 0;
};
class StateGame;
class PlayerCharacter : public CharacterInterface, public jt::Box2DObject {
public:
    PlayerCharacter(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def,
        std::weak_ptr<ItemRepository> repository, StateGame& state);

    std::shared_ptr<InventoryInterface> getInventory() override;
    std::shared_ptr<CharacterSheetImgui> getCharSheet() override;
    void handleInputMovement();
    void updateAnimation(float const elapsed);
    bool setAnimationIfNotSet(std::string const& newAnimationName);

    void gainExperience(int value);

private:
    std::shared_ptr<jt::Animation> m_animation;
    std::shared_ptr<jt::Animation> m_attackUnderlay;
    std::shared_ptr<InventoryListImgui> m_inventory;
    std::shared_ptr<CharacterSheetImgui> m_charsheet;
    StateGame& m_state;

    std::shared_ptr<SpellInterface> m_spell1;

    float m_dashTimer { -1.0f };
    float m_dashCooldown { -1.0f };
    jt::Vector2f m_dashVelocity { 0.0f, 0.0f };
    float m_attackCooldown { -1.0f };

    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void handleDashInput();
    void createAnimation();
    void handleInputAttack();
};

#endif // GUARD_JAMTEMPLATE_CHARACTER_HPP
