#include "soundManager.h"
#include <iostream>

SoundManager::SoundManager()
{
	result = FMOD::System_Create(&system);
	if (result != FMOD_OK) {
		std::cout << "¿À·ù" << std::endl;
	}

	system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	system->createSound("bgm.mp3", FMOD_LOOP_NORMAL, 0, &bgm);
	system->createSound("shooting_sound.wav", FMOD_LOOP_OFF, 0, &shooting);

}

void SoundManager::playBgm()
{
	bgm_channel->stop();
	system->playSound(bgm, 0, false, &bgm_channel);
}

void SoundManager::playShooting()
{
	//channel->stop();
	system->playSound(shooting, 0, false, &shooting_channel);
}


