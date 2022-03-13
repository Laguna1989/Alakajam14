#ifndef ALAKAJAM14_AUDIO_EVENT_QUEUE_HPP
#define ALAKAJAM14_AUDIO_EVENT_QUEUE_HPP

#include "audio_play_event.hpp"
#include "sound_interface.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>

class AudioEventQueue {
public:
    void update(float elapsed);

private:
    std::vector<AudioPlayEvent> m_buffer;

    std::map<std::string, std::shared_ptr<jt::SoundInterface>> m_sounds;
    void checkForNewEvents();
    void updateSounds(float elapsed);
};

#endif // ALAKAJAM14_AUDIO_EVENT_QUEUE_HPP
