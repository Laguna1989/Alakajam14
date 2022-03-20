#ifndef ALAKAJAM14_PLAYER_SOUND_COMPONENT_HPP
#define ALAKAJAM14_PLAYER_SOUND_COMPONENT_HPP

#include "audio/audio_interface.hpp"
#include "logging/logger_interface.hpp"
#include "sound_component_interface.hpp"

namespace jt {
class SoundGroup;
}

class PlayerSoundComponent : public SoundComponentInterface {
public:
    PlayerSoundComponent(jt::AudioInterface& audio, jt::LoggerInterface& logger);
    void play(SoundId sound) override;
    bool isPlaying(SoundId sound) override;

private:
    std::shared_ptr<jt::SoundInterface> m_soundDash;
    std::shared_ptr<jt::SoundInterface> m_soundStomp;
    std::shared_ptr<jt::SoundInterface> m_soundDeath;
    std::shared_ptr<jt::SoundGroup> m_soundGroupHurt;
};

#endif // ALAKAJAM14_PLAYER_SOUND_COMPONENT_HPP
