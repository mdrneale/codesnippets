#include "SoundManager.h"
#include <math.h>

static const char *note[120] =
{
    "C 0", "C#0", "D 0", "D#0", "E 0", "F 0", "F#0", "G 0", "G#0", "A 0", "A#0", "B 0",  
    "C 1", "C#1", "D 1", "D#1", "E 1", "F 1", "F#1", "G 1", "G#1", "A 1", "A#1", "B 1",  
    "C 2", "C#2", "D 2", "D#2", "E 2", "F 2", "F#2", "G 2", "G#2", "A 2", "A#2", "B 2",  
    "C 3", "C#3", "D 3", "D#3", "E 3", "F 3", "F#3", "G 3", "G#3", "A 3", "A#3", "B 3",  
    "C 4", "C#4", "D 4", "D#4", "E 4", "F 4", "F#4", "G 4", "G#4", "A 4", "A#4", "B 4",  
    "C 5", "C#5", "D 5", "D#5", "E 5", "F 5", "F#5", "G 5", "G#5", "A 5", "A#5", "B 5",  
    "C 6", "C#6", "D 6", "D#6", "E 6", "F 6", "F#6", "G 6", "G#6", "A 6", "A#6", "B 6",  
    "C 7", "C#7", "D 7", "D#7", "E 7", "F 7", "F#7", "G 7", "G#7", "A 7", "A#7", "B 7",  
    "C 8", "C#8", "D 8", "D#8", "E 8", "F 8", "F#8", "G 8", "G#8", "A 8", "A#8", "B 8",  
    "C 9", "C#9", "D 9", "D#9", "E 9", "F 9", "F#9", "G 9", "G#9", "A 9", "A#9", "B 9"
};


static const float notefreq[120] =
{
      16.35f,   17.32f,   18.35f,   19.45f,    20.60f,    21.83f,    23.12f,    24.50f,    25.96f,    27.50f,    29.14f,    30.87f, 
      32.70f,   34.65f,   36.71f,   38.89f,    41.20f,    43.65f,    46.25f,    49.00f,    51.91f,    55.00f,    58.27f,    61.74f, 
      65.41f,   69.30f,   73.42f,   77.78f,    82.41f,    87.31f,    92.50f,    98.00f,   103.83f,   110.00f,   116.54f,   123.47f, 
     130.81f,  138.59f,  146.83f,  155.56f,   164.81f,   174.61f,   185.00f,   196.00f,   207.65f,   220.00f,   233.08f,   246.94f, 
     261.63f,  277.18f,  293.66f,  311.13f,   329.63f,   349.23f,   369.99f,   392.00f,   415.30f,   440.00f,   466.16f,   493.88f, 
     523.25f,  554.37f,  587.33f,  622.25f,   659.26f,   698.46f,   739.99f,   783.99f,   830.61f,   880.00f,   932.33f,   987.77f, 
    1046.50f, 1108.73f, 1174.66f, 1244.51f,  1318.51f,  1396.91f,  1479.98f,  1567.98f,  1661.22f,  1760.00f,  1864.66f,  1975.53f, 
    2093.00f, 2217.46f, 2349.32f, 2489.02f,  2637.02f,  2793.83f,  2959.96f,  3135.96f,  3322.44f,  3520.00f,  3729.31f,  3951.07f, 
    4186.01f, 4434.92f, 4698.64f, 4978.03f,  5274.04f,  5587.65f,  5919.91f,  6271.92f,  6644.87f,  7040.00f,  7458.62f,  7902.13f, 
    8372.01f, 8869.84f, 9397.27f, 9956.06f, 10548.08f, 11175.30f, 11839.82f, 12543.85f, 13289.75f, 14080.00f, 14917.24f, 15804.26f
};

SoundManager::SoundManager(void)
{
//	lastpitch = 0;
//	FResult = FMOD::System_Create(&FSystem);		// Create the main system object.
//    ERRCHECK(FResult);
//
//    FResult = FSystem->getVersion(&FVersion);
//    ERRCHECK(FResult);
//
//	if (FVersion < FMOD_VERSION)
//    {
////        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
//    }
//
//
//	FResult = FSystem->setOutput(FMOD_OUTPUTTYPE_DSOUND);
//    ERRCHECK(FResult);
//	FResult = FSystem->setDriver(0);
//	ERRCHECK(FResult);
//
//    FResult = FSystem->setSoftwareFormat(OUTPUTRATE, FMOD_SOUND_FORMAT_PCM16, 1, 0, FMOD_DSP_RESAMPLER_LINEAR);
//    ERRCHECK(FResult);
//
//	FResult = FSystem->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
//    ERRCHECK(FResult);
}

SoundManager::~SoundManager(void)
{
	//for (unsigned int a = 0; a < sounds.size(); a++)
	//{
	//	sounds[a]->release();
	//	sounds[a] = NULL;
	//}
	//if (FSystem)
	//{
	//	FSystem->release();
	//	FSystem = NULL;
	//}
 }


void SoundManager::ERRCHECK(FMOD_RESULT result)
{
    //if (result != FMOD_OK)
    //{
    //    //printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
    //}
}

void SoundManager::Update				(										)
{
	//FSystem->update();
}

unsigned int SoundManager::RecordSong	( const bool Looping					)
{
	//FMOD::Sound *sound;
	//FMOD_CREATESOUNDEXINFO exinfo;
 //   memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
 //   exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
 //   exinfo.numchannels      = 1;
 //   exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
 //   exinfo.defaultfrequency = 44100;
 //   exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * 120;
 //   
 //   FResult = FSystem->createSound(0, FMOD_2D | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &sound);
	//ERRCHECK(FResult);
	//sounds.push_back(sound);

	//FResult = FSystem->recordStart(0,sound, Looping);
	//ERRCHECK(FResult);
	//return ( (unsigned int) (sounds.size()-1) );
	return 0;
}

void SoundManager::StopRecording	(										)
{
//	FSystem->recordStop(0);
}


float SoundManager::GetPitch		( unsigned int ChannelID , std::string* thenote )
{
 //   memset(&Spectrum, 0, SPECTRUMSIZE * sizeof(float));
	//FResult = channels[ChannelID]->getSpectrum(Spectrum, SPECTRUMSIZE, 0, FMOD_DSP_FFT_WINDOW_TRIANGLE);
 //   ERRCHECK(FResult);
 //   float max = 0;
	//int count;
	//int bin=0;
 //   for (count = 0; count < SPECTRUMSIZE; count++)
 //   {
 //       if (Spectrum[count] > 0.01f && Spectrum[count] > max)
 //       {
 //           max = Spectrum[count];
 //           bin = count;
 //       }
 //   }

	//if (thenote)
	//{
	//	int dominantnote = 0;
	//	for (count = 0; count < 120; count++)
	//	{
	//		 if (bin >= notefreq[count] && bin < notefreq[count + 1])
	//		 {
	//			/* which is it closer to.  This note or the next note */
	//			if (fabs(bin - notefreq[count]) < fabs(bin - notefreq[count+1]))
	//			{
	//				dominantnote = count;
	//			}
	//			else
	//			{
	//				dominantnote = count + 1;
	//			}
	//			break;
	//		 }
	//	}
	//	*thenote = note[dominantnote];
	//}
	//if (bin)
	//{
	//	lastpitch	=	(float)bin * BINSIZE;
	//	return (float)bin * BINSIZE;
	//}
	return 0;//lastpitch;
}

unsigned int SoundManager::LoadSong		( const char * const file				)
{
	//FMOD::Sound *sound;
	//FResult = FSystem->createStream(file, FMOD_SOFTWARE | FMOD_LOOP_NORMAL , 0, &sound);		// FMOD_DEFAULT uses the defaults.  These are the same as FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE.
	//ERRCHECK(FResult);
	//sounds.push_back(sound);
	//return ( (unsigned int) (sounds.size()-1) );
	return 0;
}


unsigned int SoundManager::LoadSound	( const char * const file				)
{
	return 1;
}

unsigned int SoundManager::play			( unsigned int SongID					)
{
	//FMOD::Channel *channel=NULL;
	//FResult = FSystem->playSound(FMOD_CHANNEL_REUSE, sounds[SongID], false, &channel);
	//ERRCHECK(FResult);
	//channels.push_back(channel);
	//return ( (unsigned int) (channels.size()-1) );
	return 0;
}

void SoundManager::pause				( unsigned int ChannelID					)
{
//	FMOD::Channel *channel;
//	FResult = FSystem->getChannel(SpeakerID,&channel);
//	ERRCHECK(FResult);
//	bool ispaused;
//	channels[ChannelID]->getPaused(&ispaused);
//	channels[ChannelID]->setPaused(!ispaused);
}

void SoundManager::SetSpeed				( unsigned int ChannelID, float Speed	)
{
//	FMOD::Channel *channel;
//	FResult = FSystem->getChannel(SpeakerID,&channel);
//	ERRCHECK(FResult);
//	FResult = channels[ChannelID]->setFrequency(Speed);
//	ERRCHECK(FResult);
}

void SoundManager::SetPosition			( unsigned int ChannelID, float Position	)
{
//	FMOD::Channel *channel;
//	FResult = FSystem->getChannel(SpeakerID,&channel);
//	ERRCHECK(FResult);
//	unsigned int Pos = (unsigned int)(Position * 1000.0f); 
//	channels[ChannelID]->setPosition(Pos,FMOD_TIMEUNIT_MS);
}

float SoundManager::Getspeed			( unsigned int ChannelID				)
{
//	FMOD::Channel *channel;
//	FResult = FSystem->getChannel(SpeakerID,&channel);
//	ERRCHECK(FResult);
//	float freq;
//	FResult = channels[ChannelID]->getFrequency(&freq);
//	ERRCHECK(FResult);
//	return freq;
	return 0;
}

float SoundManager::GetPosition			( unsigned int ChannelID				)
{
//	FMOD::Channel *channel;
//	FResult = FSystem->getChannel(SpeakerID,&channel);
//	ERRCHECK(FResult);
//	unsigned int Pos;
//	channels[ChannelID]->getPosition(&Pos,FMOD_TIMEUNIT_MS);
//	return ( ( (float) Pos ) * 0.001f );
	return 0;
}

void	SoundManager::SetVolume		( unsigned int ChannelID, float Volume	)
{
	//if (ChannelID < (unsigned int)channels.size() )
	//{
	//	channels[ChannelID]->setVolume(Volume);
	//}
}
float	SoundManager::GetVolume		( unsigned int ChannelID				)
{
	//float vol=0;
	//channels[ChannelID]->getVolume(&vol);
	//return vol;
	return 0;
}