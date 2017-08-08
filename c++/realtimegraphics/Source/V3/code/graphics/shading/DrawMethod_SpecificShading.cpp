#include "DrawMethod_SpecificShading.h"
#include "..\VisualInterface.h"
#include "DrawMethod_DebugOutput.h"

enum BlendTypes
{
	BT_Multiply = 0,
	BT_Add		= 1
};

DrawMethod_SpecificShading::DrawMethod_SpecificShading(LPDIRECT3DDEVICE9 device,SceneData *sd)
{
	d3DDevice = device;
	sceneData = sd;

	PositionPass	= NULL;
	NormalPass		= NULL;
	ColourPass		= NULL;
	ShadowPass		= NULL;
}

void DrawMethod_SpecificShading::RenderScene(DM_Output* out)
{
	if (out)
	{
		for (unsigned int a = 0; a<sceneData->Models.size(); a++)
		{
			if (sceneData->Models[a]->EffectID!=-1)
			{
				int effid=sceneData->Models[a]->EffectID;
				if (sceneData->effects[effid]->Effect)
				{
					LPD3DXEFFECT objectsEffect = sceneData->effects[effid]->Effect;
					if (sceneData->effects[effid]->NeedsPosMap && PositionPass)
					{
						objectsEffect->SetTexture("PositionTexture",PositionPass);
					}
					if (sceneData->effects[effid]->NeedsColMap && ColourPass)
					{
						objectsEffect->SetTexture("ColourTexture",ColourPass);
					}
					if (sceneData->effects[effid]->NeedsNormMap	&& NormalPass)
					{
						objectsEffect->SetTexture("NormalTexture",NormalPass);
					}
					if (sceneData->effects[effid]->BlendType==BT_Multiply)
					{
//						d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//						d3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
//						d3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
					}
					Render(objectsEffect,out,sceneData->Models[a]);
					d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				}
			}
		}
	}
}

void DrawMethod_SpecificShading::Render		(LPD3DXEFFECT Effect,DM_Output* out,Model* model)
{
	HRESULT hr;
	d3DDevice->SetRenderTarget(0,out->Render);
	d3DDevice->SetDepthStencilSurface(out->Depth);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		SetWorldMatrix(*model,Effect);
		SetVeiwMatrix(*sceneData->Cameras[sceneData->CurrentCamera],Effect);
		SetProjectionMatrix(sceneData->Cameras[sceneData->CurrentCamera]->FOV,0.10f,1000.0f,Effect);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = Effect->GetTechniqueByName("Standard");
		if (SUCCEEDED(Effect->SetTechnique(hTech)))
		{
			Effect->Begin(&Passes,0);
			Effect->BeginPass(0);
			hr = d3DDevice->SetStreamSource(0,sceneData->Meshes[model->MeshID]->_vb,0,44 );
			hr = d3DDevice->SetIndices(sceneData->Meshes[model->MeshID]->_ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,sceneData->Meshes[model->MeshID]->NumofVerts,0,sceneData->Meshes[model->MeshID]->PrimitiveCount);
			Effect->EndPass();
			Effect->End();
		}
	}
	hr = d3DDevice->EndScene();
}