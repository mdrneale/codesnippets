#ifndef SoundManager_h
#define SoundManager_h

#include <vector>
#include "fmod\fmod.hpp"
#include "fmod\fmod_errors.h"

#define OUTPUTRATE          48000
#define SPECTRUMSIZE        8192
#define SPECTRUMRANGE       ((float)OUTPUTRATE / 2.0f)      /* 0 to nyquist */

#define BINSIZE      (SPECTRUMRANGE / (float)SPECTRUMSIZE)

class SoundManager
{
public:
	SoundManager					(														);
   ~SoundManager					(														);

	void			Update			(														);

	unsigned int	LoadSong		( const char * const file								);
	unsigned int	LoadSound		( const char * const file								);

	unsigned int	RecordSong		( const bool Looping									);
	void			StopRecording	(														);

	unsigned int	play			( unsigned int SongID									);
	void			pause			( unsigned int ChannelID								);
	void			SetVolume		( unsigned int ChannelID, float Volume					);
	float			GetVolume		( unsigned int ChannelID								);

	void			SetSpeed		( unsigned int ChannelID, float Speed					);
	void			SetPosition		( unsigned int ChannelID, float Position				);

	float			Getspeed		( unsigned int ChannelID								);
	float			GetPosition		( unsigned int ChannelID								);
	float			GetPitch		( unsigned int ChannelID , std::string* thenote = NULL	);

private:

    void			ERRCHECK		( FMOD_RESULT result									);

	FMOD::System				   *FSystem;
  	std::vector<FMOD::Sound*>		sounds;
    std::vector<FMOD::Channel*>		channels;
	float							lastpitch;
    FMOD_RESULT						FResult;
    int								key;
    unsigned int					FVersion;
	float							Spectrum[SPECTRUMSIZE];
};


#endif