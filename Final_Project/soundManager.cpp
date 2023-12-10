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


}

void SoundManager::playBgm()
{
	channel->stop();
	system->playSound(bgm, 0, false, &channel);
}

void SoundManager::playShooting()
{
	
}


