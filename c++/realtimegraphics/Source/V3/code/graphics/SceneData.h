#ifndef SCENEDATA_H
#define SCENEDATA_H

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include "model/Model.h"
#include "mesh/mesh.h"
#include "material/Material.h"
#include "lights/Light.h"
#include "camera/Camera.h"
#include "colour/Colour.h"
#include "EffectData.h"

struct SceneData
{
	std::vector<Model*>				Models;
	std::vector<Mesh*>				Meshes;
	std::vector<Material*>			Materials;
	std::vector<EffectData*>		effects;
	std::vector<Camera*>			Cameras;
	std::vector<Light*>				Lights;
	std::vector<LPDIRECT3DTEXTURE9>	Textures;
	unsigned int					CurrentCamera;
	Colour							AmbientColour;
};
#endif