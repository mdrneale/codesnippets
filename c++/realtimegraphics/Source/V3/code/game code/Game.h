#ifndef game_h
#define game_h
#include "..\graphics\VisualInterface.h"
#include "..\input\InputInterface.h"
#include "..\sound\SoundInterface.h"
#include "..\VisualEffects\EffectsSystem.h"
#include "..\VisualEffects\FireWorkEffect.h"

enum E_Messages
{
	E_NoMessages		= 0,
	E_Exit				= 1,
	E_NumberOfMessages 
};

enum E_Stage
{
	E_StartScreen,
	E_Recording,
	E_NumberOfStages
};

class game
{
public:
//constructor, destructor and initalise function
	 game(void);
	~game(void);
	 bool			Initalise			( const bool fullscreen = false	);
	 void			Update				(								);
	 void			SetFps				( const int fps					);
	 unsigned int	GetFps				(								) { return FPS;	}
	 unsigned char	GetMessages			(								) { return MSG;	}

private:
	void			LoadLevel			( const char * const file		);
	void			LoadKeys			( const char * const file		);
	void			LoadGlobalTextures	(								);

private:

	//frame info
	unsigned int	FPS;
	unsigned int	HighestFPS;
	unsigned int	LowestFPS;
	unsigned int	AverageFPS;
	unsigned int	GameRunTime;

	unsigned char	MSG;
	unsigned char	STG;

	VisualInterface		*Vinterface;
	inputinterface		*Iinterface;
	SoundInterface		*Sinterface;
	EffectsSystem		*ESinterface;

	FireWorkEffect			te;
	FireWorkEffect			te2;
	FireWorkEffect			te3;


};


#endif