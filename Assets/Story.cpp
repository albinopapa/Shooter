#include "Story.h"


void PlaySnd( int nParams, ... ) // (char* filename, int volume, Story* story)
{
	va_list list;
	va_start( list, nParams );
	char* filename = va_arg( list, char* );
	int volume = va_arg( list, int );
	Story* story = va_arg( list, Story* );
	va_end( list );
	story->Play( filename, volume );
}

void StopSnd( int nParams, ... ) // (char* filename, Story* story)
{
	va_list list;
	va_start( list, nParams );
	char* filename = va_arg( list, char* );
	Story* story = va_arg( list, Story* );
	va_end( list );
	story->StopSound( filename );
}

Story::Story(StateCore& core)
	:
	core(core),
	sound()
{
	
}

Story::~Story()
{}

void Story::CreateSound(char* wavFileName)
{
	sounds.push_back( sound );
	sounds[ sounds.size() - 1 ].sound = core.audio.CreateSound( wavFileName );
	sounds[ sounds.size() - 1 ].name = wavFileName;
}

void Story::Play(char* wavFileName, float volume ) // 1 for full
{
	for(std::vector<SoundInfo>::iterator it = sounds.begin(); it != sounds.end(); it++)
	{
		if( wavFileName == it->name )
		{
			it->sound.Play(volume - 1);
		}
	}
}

void Story::StopSound(char* wavFileName)
{
	for(std::vector<SoundInfo>::iterator it = sounds.begin(); it != sounds.end(); it++)
	{
		if( wavFileName == it->name )
		{
			it->sound.Stop();
		}
	}
}

