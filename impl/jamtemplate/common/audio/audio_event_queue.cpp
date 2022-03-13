#include "audio_event_queue.hpp"
#include "sound.hpp"
void AudioEventQueue::update(float elapsed)
{
    checkForNewEvents();
    updateSounds(elapsed);
}

void AudioEventQueue::updateSounds(float elapsed)
{
    for (auto& kvp : m_sounds) {
        kvp.second->update(elapsed);
    }
}

void AudioEventQueue::checkForNewEvents()
{
    while (!m_buffer.empty()) {
        auto event = m_buffer.back();
        m_buffer.pop_back();

        auto const fileName = event.AudioFileName;
        if (m_sounds.count(fileName) == 0) {
            auto snd = std::make_shared<jt::Sound>(fileName);
            m_sounds[fileName] = snd;
        }
        m_sounds[fileName]->play();
    }
}
