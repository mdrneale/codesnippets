#include "SoundInterface.h"

//singleton
SoundInterface *	SoundInterface::Instance = 0;

SoundInterface *	SoundInterface::I		(												)
{
	if ( !Instance )
	{
		Instance = new SoundInterface();
	}
	return Instance;
}
SoundInterface::SoundInterface				(												)
{

}

SoundInterface::~SoundInterface				(												)
{
	if ( Instance )
	{
		Instance=0;
	}
}

void SoundInterface::Update					(												)
{
	sndmanager.Update();
}

unsigned int SoundInterface::LoadSong		( const char * const file						)
{
	return sndmanager.LoadSong(file);
}

unsigned int SoundInterface::LoadSound		( const char * const file						)
{
	return 1;
}

void SoundInterface::play					( unsigned int SongID							)
{
	sndmanager.play(SongID);
}

void SoundInterface::pause					( unsigned int SongID							)
{
	sndmanager.pause(SongID);
}
void SoundInterface::SetSpeed				( unsigned int SongID, float Speed				)
{
	sndmanager.SetSpeed(SongID,Speed);
}
void SoundInterface::SetPosition			( unsigned int SongID, float Position			)
{
	sndmanager.SetPosition(SongID, Position);
}
float SoundInterface::Getspeed				( unsigned int SongID							)
{
	return sndmanager.Getspeed(SongID);
}

float SoundInterface::GetPosition			( unsigned int SongID							)
{
	return sndmanager.GetPosition(SongID);
}

unsigned int SoundInterface::RecordSong		( const bool Looping							)
{
	return sndmanager.RecordSong( Looping );
}

void SoundInterface::StopRecording			(												)
{
	sndmanager.StopRecording();
}

float SoundInterface::GetPitch				( unsigned int ChannelID, std::string* thenote	)
{
	return sndmanager.GetPitch( ChannelID, thenote );
}

void SoundInterface::SetVolume				( unsigned int ChannelID, float Volume			)
{
	sndmanager.SetVolume(ChannelID, Volume	);
}
float SoundInterface::GetVolume				( unsigned int ChannelID						)
{
	return sndmanager.GetVolume(ChannelID);
}