﻿#ifndef GUARD_JAMTEMPLATE_GAMEBASE_HPP_GUARD
#define GUARD_JAMTEMPLATE_GAMEBASE_HPP_GUARD

#include "game_interface.hpp"
#include "game_object.hpp"
#include <chrono>
#include <memory>

namespace jt {
class GameBase : public GameInterface,
                 public GameObject,
                 public std::enable_shared_from_this<GameBase> {
public:
    GameBase(GfxInterface& gfx, InputManagerInterface& input, AudioInterface& audio,
        StateManagerInterface& stateManager, LoggerInterface& logger,
        ActionCommandManagerInterface& actionCommandManager);

    void runOneFrame() override;

    GfxInterface& gfx() const override;

    InputManagerInterface& input() override;

    AudioInterface& audio() override;

    StateManagerInterface& stateManager() override;

    LoggerInterface& logger() override;

    ActionCommandManagerInterface& actionCommandManager() override;

    void reset() override;

    void cheat() override;
    bool wasCheating() override;

protected:
    std::weak_ptr<GameInterface> getPtr() override;

    // overwritten functions from GameObject
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    GfxInterface& m_gfx;

    InputManagerInterface& m_inputManager;

    AudioInterface& m_audio;

    StateManagerInterface& m_stateManager;

    LoggerInterface& m_logger;

    ActionCommandManagerInterface& m_actionCommandManager;

    std::chrono::steady_clock::time_point m_timeLast {};

    float m_lag { 0.0f };
    float m_timePerUpdate { 0.005f };
    int m_maxNumberOfUpdateIterations { 100 };
    bool m_wasCheating { false };
};

} // namespace jt

#endif
