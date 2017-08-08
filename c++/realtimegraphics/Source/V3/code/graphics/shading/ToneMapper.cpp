#include "ToneMapper.h"

ToneMapper::ToneMapper()
{
	UpdateLuminanceTimer.Duration			= 1.0f;
	UpdateCurrentLuminanceTimer.Duration	= 0.05f;
	CurrentLuminance						= 0.5f;
	VisibleLightSize						= 1.0f;
	LuminanceToReach						= 0.5f;
	MaximumLuminanceLevel					= 5.0f;
	MinimumLuminanceLevel					= 0.0f;
	LightAdjustSpeed						= 0.8f;
}

ToneMapper::RenderScene(LPDIRECT3DTEXTURE9 Luminance, DM_Output *outputRender)
{
	if (UpdateLuminanceTimer.Check())
	{
		LuminanceToReach = GetLuminance(Luminance);
	}
	float time=0;
	if (UpdateCurrentLuminanceTimer.Check(time))
	{
		float Ldx = LuminanceToReach - CurrentLuminance;
		CurrentLuminance=CurrentLuminance+(Ldx*time*LightAdjustSpeed);
		if (CurrentLuminance>MaximumLuminanceLevel)
		{
			CurrentLuminance=MaximumLuminanceLevel;
		}
		if (CurrentLuminance<MinimumLuminanceLevel)
		{
			CurrentLuminance=MinimumLuminanceLevel;
		}
	}
}

float	ToneMapper::GetLuminance(LPDIRECT3DTEXTURE9 Luminance)
{
	return 0.5f;
}

void	ToneMapper::Render(DM_Output* outputRender)
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
