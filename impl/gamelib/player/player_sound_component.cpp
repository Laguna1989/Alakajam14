#include "player_sound_component.hpp"
#include "audio/logging_sound.hpp"
#include "audio/sound.hpp"
#include "audio/sound_group.hpp"

PlayerSoundComponent::PlayerSoundComponent(jt::AudioInterface& audio, jt::LoggerInterface& logger)
{
    m_soundDash = std::make_shared<jt::LoggingSound>(
        std::make_shared<jt::Sound>("assets/sound/attack_dash_3.ogg"), logger);
    m_soundDash->setVolume(0.4f);
    audio.addTemporarySound(m_soundDash);

    m_soundStomp = std::make_shared<jt::LoggingSound>(
        std::make_shared<jt::Sound>("assets/sound/attack_stomp.ogg"), logger);
    m_soundStomp->setVolume(0.4f);
    audio.addTemporarySound(m_soundStomp);

    m_soundDeath = std::make_shared<jt::LoggingSound>(
        std::make_shared<jt::Sound>("assets/sound/GAME_OVER.ogg"), logger);
    audio.addTemporarySound(m_soundDeath);

    auto const soundHurt1 = std::make_shared<jt::LoggingSound>(
        std::make_shared<jt::Sound>("assets/sound/hit_squishy_sound_01.ogg"), logger);
    auto const soundHurt2 = std::make_shared<jt::LoggingSound>(
        std::make_shared<jt::Sound>("assets/sound/hit_squishy_sound_02.ogg"), logger);
    auto const soundHurt3 = std::make_shared<jt::LoggingSound>(
        std::make_shared<jt::Sound>("assets/sound/hit_squishy_sound_03.ogg"), logger);
    auto const soundHurt4 = std::make_shared<jt::LoggingSound>(
        std::make_shared<jt::Sound>("assets/sound/hit_squishy_sound_04.ogg"), logger);
    auto const soundHurt5 = std::make_shared<jt::LoggingSound>(
        std::make_shared<jt::Sound>("assets/sound/hit_squishy_sound_05.ogg"), logger);

    soundHurt1->setVolume(0.6f);
    soundHurt2->setVolume(0.6f);
    soundHurt3->setVolume(0.6f);
    soundHurt4->setVolume(0.6f);
    soundHurt5->setVolume(0.6f);

    m_soundGroupHurt = std::make_shared<jt::SoundGroup>();
    m_soundGroupHurt->add(soundHurt1);
    m_soundGroupHurt->add(soundHurt2);
    m_soundGroupHurt->add(soundHurt3);
    m_soundGroupHurt->add(soundHurt4);
    m_soundGroupHurt->add(soundHurt5);
    audio.addTemporarySound(m_soundGroupHurt);
}

void PlayerSoundComponent::play(SoundComponentInterface::SoundId sound)
{
    // TODO figure out why we need to stop certain sounds before playing
    if (sound == SoundId::DASH) {
        m_soundDash->stop();
        m_soundDash->play();
    } else if (sound == SoundId::STOMP) {
        m_soundStomp->stop();
        m_soundStomp->play();
    } else if (sound == SoundId::HURT) {
        m_soundGroupHurt->play();
    } else if (sound == SoundId::DEATH) {
        m_soundDeath->play();
    }
}

bool PlayerSoundComponent::isPlaying(SoundComponentInterface::SoundId sound)
{
    if (sound == SoundId::DASH) {
        return m_soundDash->isPlaying();
    } else if (sound == SoundId::STOMP) {
        return m_soundStomp->isPlaying();
    } else if (sound == SoundId::HURT) {
        return m_soundGroupHurt->isPlaying();
    } else if (sound == SoundId::DEATH) {
        return m_soundDeath->isPlaying();
    }
    return false;
}
