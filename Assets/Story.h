#pragma once
#include "StateCore.h"
#include "StandardLibraryIncludes.h"


class Story
{
public:
	struct SoundInfo
	{
		Sound sound;
		char* name;
	};
	struct Params
	{
		void* param;
		std::string name;
	};
	Story( StateCore& core );
	~Story();
	void CreateSound(char* wavFileName);
	void Play(char* wavFileName, float volume);
	void StopSound(char* wavFileName);
private:
	SoundInfo sound;
	std::vector<SoundInfo> sounds;
	StateCore& core;
};

void PlaySnd( int nParams, ... ); // (char* filename, int volume, Story* story)

void StopSnd( int nParams, ... ); // (char* filename, Story* story)
