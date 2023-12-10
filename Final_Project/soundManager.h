#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"

class SoundManager
{
public:
	FMOD::System* system;
	FMOD::Sound* bgm, * shooting;
	FMOD::Channel* bgm_channel = 0, * shooting_channel = 0;
	FMOD_RESULT result;
	void* extradriverdata = 0;

	SoundManager();
	void playBgm();
	void playShooting();
};