#ifndef SoundInterface_h
#define SoundInterface_h

#include "SoundManager.h"

class SoundInterface
{
public:
							   ~SoundInterface	(														);

	void						Update			(														);

	unsigned int				LoadSong		( const char * const file								);
	unsigned int				LoadSound		( const char * const file								);

	unsigned int				RecordSong		( const bool Looping									);
	void						StopRecording	(														);
	float						GetPitch		( unsigned int ChannelID, std::string* thenote = NULL	);

	void						SetVolume		( unsigned int ChannelID, float Volume					);
	float						GetVolume		( unsigned int ChannelID								);
	void						play			( unsigned int SongID									);
	void						pause			( unsigned int SongID									);

	void						SetSpeed		( unsigned int SongID, float Speed						);
	void						SetPosition		( unsigned int SongID, float Position					);

	float						Getspeed		( unsigned int SongID									);
	float						GetPosition		( unsigned int SongID									);

private:
								SoundInterface	(														);

	static SoundInterface *		Instance;
	SoundManager				sndmanager;
public:
	static SoundInterface *		I				(														);
};

#endif