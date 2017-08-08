#ifndef VISUALINTERFACE
#define VISUALINTERFACE
#include <d3d9.h>
#include <d3dx9.h>
#include "SceneData.h"
#include <vector>
#include "model/Model.h"
#include "mesh/mesh.h"
#include "material/Material.h"
#include "lights/Light.h"
#include "camera/Camera.h"
#include "shading\DrawMethod.h"
#include "shading\DM_Output.h"
//#include "../../HeadTrack/HeadTrack.h"

#define	WIDTH	1280
#define	HEIGHT	800

class VisualInterface
{
public:
	 VisualInterface();
	~VisualInterface();
	static VisualInterface*	I	(													);

	bool Initialise				( const bool fscreen = false						);

	void Update					(													);
	
	void Render					(													);

	int loadmesh				( const char* FileName								);

	int LoadTexture				( const char* FileName								);

	int addmodel				( Model& ModelToAdd									);

	int AddCamera				( Camera& CamToAdd									);

	int AddLight				( Light& LightToAdd									);

	void GetLight				( int lightID , Light** light						);

	void GetCamera				( Camera** Cam										);

	void NextCamera				(													);

	void SetBackgroundColour	( const BYTE red, const BYTE green,	const BYTE blue	);

	void SetProjection			( float FOV, float nearPlane, float farPlane		);

	void SetVertexDeclaration	(													);

	bool UsingHeadTracking		( float xyz[3]										);
	
	LPDIRECT3DDEVICE9			Naughty(){return gD3dDevice;};

	void makeit();

	int LightBox;
private:

	void Set3Dcam();
//	HeadTracker					ht;
	float XYZ[3];
	LPDIRECT3DDEVICE9		gD3dDevice;
	static VisualInterface*	Instance;
	// startup function
	bool InitialiseDirect3D		(													);		
	void InitialiseVertexDeclaration	(											);

	// shutdown function
	void CloseDownDirect3D		(								);		

	int  LoadEffect				(	const char* filename		);
	void LoadScene				(	const char* FileName		);

	void RenderModels			(								);
	void RenderModel			(	Model* m					);

	//D3D pointers
	LPDIRECT3D9						gD3dObject;				
	IDirect3DVertexDeclaration9	*	vd; 
	int								VertexSize;

	SceneData						scene;
	std::vector<DrawMethod*>		drawMethods;
	DM_Output						drawMethodOutput;
	//boolean for initialize check
	bool							isinitialised;			
	//boolean for windowed or full screen
	bool							isfullscreen;
	//background colour
	BYTE							r;  
	BYTE							g;  
	BYTE							b;					
	//screen resolution
	unsigned int					ScreenWidth;
	unsigned int					ScreenHeight;

};

#endif