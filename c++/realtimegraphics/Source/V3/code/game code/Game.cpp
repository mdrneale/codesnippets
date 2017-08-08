#include "game.h"
#include "..\xml\XmlInterface.h"
#include "..\helper code\HelperCode.h"
#include "..\helper code\Timer.h"

game::game(void):FPS(1),HighestFPS(1),LowestFPS(100000),AverageFPS(0),GameRunTime(0),MSG(0),STG(E_StartScreen)
{

}

game::~game(void)
{
	XmlInterface	gamedata;

	XmlElement	*	xmlgame		=	new XmlElement();
	xmlgame->SetName( "gamedata" );

	XmlElement	*	xmlfps		=	new XmlElement();
	xmlfps->SetName( "highestfps" );
	xmlfps->SetValue( ToString(HighestFPS).c_str() );

	xmlgame->AddChild( xmlfps );

	XmlElement	*	xmllowfps		=	new XmlElement();
	xmllowfps->SetName( "lowestfps" );
	xmllowfps->SetValue( ToString(LowestFPS).c_str() );

	xmlgame->AddChild( xmllowfps );

	XmlElement	*	xmlavfps		=	new XmlElement();
	xmlavfps->SetName( "averagefps" );
	if (GameRunTime)
	{
		xmlavfps->SetValue( ToString(AverageFPS/GameRunTime).c_str() );
	}
	xmlgame->AddChild( xmlavfps );

	gamedata.SaveXml("xml\\statistics\\gamedata.xml",xmlgame);


	delete xmlgame;

	delete Vinterface;
	delete Iinterface;
	delete Sinterface;
}

void game::SetFps( const int fps ) 
{
	++GameRunTime;
	FPS = fps;
	if ( FPS > HighestFPS )
	{
		HighestFPS = FPS;
	}
	if ( FPS < LowestFPS && FPS != 1 )
	{
		LowestFPS = FPS;
	}
	AverageFPS+=FPS;
}

bool game::Initalise(const bool fullscreen)
{
	Iinterface = inputinterface::I();
	Vinterface = VisualInterface::I();
	Sinterface = SoundInterface::I();
	ESinterface= EffectsSystem::I();
	if (!Vinterface->Initialise(fullscreen)) 
	{ 
		return false; 
	}
	Vinterface->SetBackgroundColour(95,95,95);
	LoadKeys("xml\\input\\move.xml");
	Vinterface->loadmesh("C:\\Users\\b2039521\\Desktop\\plane.obj");
	Camera cam;
	cam.Position.x=0;
	cam.Position.y=10;
	cam.Position.z=0;
	cam.Interest.x=20;
	cam.Interest.y=0;
	cam.Interest.z=0;
	cam.Up.x=0;
	cam.Up.y=1.0f;
	cam.Up.z=0;
	cam.FOV=D3DX_PI / 4.0f;
	Vinterface->AddCamera(cam);
	cam.Position.x=0;
	cam.Position.y=10;
	cam.Position.z=0;
	cam.Interest.x=20;
	cam.Interest.y=0;
	cam.Interest.z=0;
	cam.Up.x=0;
	cam.Up.y=1.0f;
	cam.Up.z=0;
	Vinterface->AddCamera(cam);

	Light alight;
	alight.LightBox	= Vinterface->LightBox;
	alight.Position.x=0.0f;
	alight.Position.y=65.0f;
	alight.Position.z=0.0f;
	alight.Interest.x=0.0f;
	alight.Interest.y=0.0;
	alight.Interest.z=0.0f;
	alight.Up.x=1.0f;
	alight.Up.y=0.0f;
	alight.Up.z=0.0f;
	alight.FOV		= D3DX_PI / 2.0f;
	alight.InnerFOV	= D3DX_PI / 3.0f;
	alight.Col.B=30.0f;
	alight.Col.G=40.0f;
	alight.Col.R=45.0f;
	alight.Attenuation = D3DXVECTOR3(0.25f,0.12f,0.07f);
	Vinterface->AddLight(alight);

	alight.Position.x=-1.0;
	alight.Position.y=20.0f;
	alight.Position.z=1.0f;
	alight.Interest.x=-20.0f;
	alight.Interest.y=0.0;
	alight.Interest.z=-20.0f;
	alight.Up.x=0;
	alight.Up.y=1.0f;
	alight.Up.z=0.0f;
	alight.FOV=D3DX_PI / 2.0f;
	alight.Col.B=20.0f;
	alight.Col.G=20.0f;
	alight.Col.R=30.0f;
	//	alight.Attenuation(0.5f,0.2f,0.7f);
	Vinterface->AddLight(alight);

	alight.Position.x=1.0;
	alight.Position.y=20.0f;
	alight.Position.z=-1.0f;
	alight.Interest.x=20.0f;
	alight.Interest.y=0.0;
	alight.Interest.z=-20.0f;
	alight.Up.x=0;
	alight.Up.y=1.0f;
	alight.Up.z=0.0f;
	alight.FOV=D3DX_PI / 2.0f;
	alight.Col.B=20.0f;
	alight.Col.G=35.0f;
	alight.Col.R=20.0f;
	//	alight.Attenuation(0.5f,0.2f,0.7f);
	Vinterface->AddLight(alight);

	alight.Position.x=1.0;
	alight.Position.y=20.0f;
	alight.Position.z=1.0f;
	alight.Interest.x=-20.0f;
	alight.Interest.y=0.0;
	alight.Interest.z=20.0f;
	alight.Up.x=0;
	alight.Up.y=1.0f;
	alight.Up.z=0.0f;
	alight.FOV=D3DX_PI / 2.0f;
	alight.Col.B=28.0f;
	alight.Col.G=20.0f;
	alight.Col.R=20.0f;
	//	alight.Attenuation(0.5f,0.2f,0.7f);
	Vinterface->AddLight(alight);

//	ESinterface->Initalise();
//	FireWorkEffectData ted;
//	ted.pos = D3DXVECTOR3(0,0,0);
//	ted.velosity.x = 20.0f;
//	ted.velosity.y = 40.0f;
//	ted.velosity.z = 20.0f;
//	ted.TimeTillBoom=4.0f;

	//ESinterface->Add(&te,&ted);

//	ESinterface->Update();
	return true;
}


void game::LoadKeys(const char * const file)
{
	Iinterface->loadkeys(file);
}

void game::LoadLevel(const char * const level)
{	
}

void game::Update()
{
	Vinterface->Update();
//	ESinterface->Update();
	Iinterface->Update();
	Sinterface->Update();

	static Timer T;
	T.Duration = 0.01f;

	static Timer lightmove;
	lightmove.Duration = 0.005f;
	if (lightmove.Check())
	{
		static float sf=0.0f;
		sf+=0.005f;
		Light *l1;
		Vinterface->GetLight(0,&l1);
		//l1->Position.x=30+40.0f*sin(sf*0.2f);
		//l1->Position.y=40.0f;
		//l1->Position.z=10.0f*cos(sf*0.5f);
//		l1->Interest.x=l1->Position.x-2.0f*sin(sf);
//		l1->Interest.y=0.0f;
//		l1->Interest.z=l1->Position.z-10.0f*cos(sf-0.2f);

		Vinterface->GetLight(1,&l1);
		l1->Position.x=20+40.0f*sin(sf*0.3f);
		l1->Position.y=50.0f;
		l1->Position.z=10.0f*cos(sf*0.25f);
		l1->Interest.x=l1->Position.x-2.0f*sin(sf);
		l1->Interest.y=50.0f;
		l1->Interest.z=l1->Position.z-10.0f*cos(sf+0.25f);

		Vinterface->GetLight(2,&l1);
		//l1->Position.x=40.0f*sin(sf*0.2f);
		l1->Position.y=40.0f;
		l1->Position.z=10.0f*cos(sf*0.25f);
		l1->Interest.x=l1->Position.x-2.0f*sin(sf);
		l1->Interest.y=20.0f;
		//l1->Interest.z=l1->Position.z-10.0f*cos(sf-0.2f);
		Camera* t=NULL;
		Vinterface->GetCamera(&t);
		l1->Interest	= l1->Position;
		l1->Up			= t->Up;

		Vinterface->GetLight(3,&l1);
		l1->Position.x=60.0f*sin(sf);
		l1->Position.y=30.0f;
		l1->Position.z=12+2.0f*cos(sf*0.25f);
		l1->Interest.x=l1->Position.x-4.0f*sin(sf*0.5f);
		l1->Interest.y=0.0f;
		l1->Interest.z=(l1->Position.z+1.0f)+10.0f*abs(cos(sf+0.35f));

//		l1->Interest.x=-30.0f*sin(sf+0.2f);
//		l1->Interest.y=0.0f;
//		l1->Interest.z=-10.0f*cos(sf-0.2f);

//		l1->Interest.x = 20.0f;//*sin(1*sf);
//		l1->Interest.z = 20.0f*cos(1*sf);
	//	l1->Col.R = 5*(sin(sf)*0.5f+0.5f);
	//	l1->Col.G = 5*(cos(2*sf)*0.5f+0.5f);
	//	l1->Position.y = 4.2f+(0.5f*sin(sf*20.0f));
	//	l1->Position.z = 2.0f*sin(sf*4.0f);
	//	l1->Position.x = 2.0f*cos(sf*4.0f);
	//	Vinterface->GetLight(2,&l1);
	//	l1->Position.y = 2+2.0f*cos(sf*3.0f);
	//	Vinterface->GetLight(3,&l1);
	//	l1->Position.x = 2.0f*cos(sf*0.20f);
	//	l1->Position.z = 1.3f+2.0f*sin(sf*0.2f);
	//	Camera*c;
	//	Vinterface->GetCamera(&c);
	//	l1->Position = c->Position;
	}
//	Camera* t=NULL;
//	Vinterface->GetCamera(&t);
//	t->Position.x = 4.0f*sin(sf);
//	t->Position.z = 4.0f*cos(sf);

	if (T.Check())
	{
		static bool scam=false;
		int pscam = (int)Iinterface->getaction("switch cam");
		if (pscam && !scam)
		{
			Light* l1;
			static int a=0;
			Vinterface->GetLight(0,&l1);
			Camera* t=NULL;
			Vinterface->GetCamera(&t);

			l1->Position	= t->Position;
			l1->Interest	= t->Interest;
			l1->Up			= t->Up;

//			Vinterface->NextCamera();
			scam = true;
		}
		else if (!pscam)
		{
			scam = false;
		}
		if (Iinterface->getaction("move forward"))
		{
			Camera* t=NULL;
			Vinterface->GetCamera(&t);
			t->MoveForward(0.4f);
		}
		if (Iinterface->getaction("move backward"))
		{
			Camera* t=NULL;
			Vinterface->GetCamera(&t);
			t->MoveForward(-0.4f);
		}
		if (Iinterface->getaction("move left"))
		{
			Camera* t=NULL;
			Vinterface->GetCamera(&t);
			t->MoveRight(0.4f);
		}
		if (Iinterface->getaction("move right"))
		{
			Camera* t=NULL;
			Vinterface->GetCamera(&t);
			t->MoveRight(-0.4f);
		}

		float a = Iinterface->getaction("turn up");
		if (a)
		{
			Camera* t=NULL;
			Vinterface->GetCamera(&t);
			t->Rotate(0.0f,0.0f-a*0.05f);
		}
		float d = Iinterface->getaction("turn down");
		if (d)
		{
			Camera* t=NULL;
			Vinterface->GetCamera(&t);
			t->Rotate(0.0f,(d)*0.05f);
		}
		float l = Iinterface->getaction("turn right");
		if (l)
		{
			Camera* t=NULL;
			Vinterface->GetCamera(&t);
			t->Rotate(l*0.05f,0.0f);
		}
		float r = Iinterface->getaction("turn left");
		if (r)
		{
			Camera* t=NULL;
			Vinterface->GetCamera(&t);
			t->Rotate((0.0f-r)*0.05f,0.0f);
		}

	}

	if (Iinterface->getaction("quit"))	{	MSG |= E_Exit;	}
	Vinterface->Render();
}