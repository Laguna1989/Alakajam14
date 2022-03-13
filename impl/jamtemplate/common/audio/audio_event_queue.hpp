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
    void update();
    void addEvent(AudioPlayEvent const& event);

private:
    std::vector<AudioPlayEvent> m_buffer;

    std::map<std::string, std::shared_ptr<jt::SoundInterface>> m_sounds;
    void checkForNewEvents();
    void updateSounds();
};

#endif // ALAKAJAM14_AUDIO_EVENT_QUEUE_HPP
