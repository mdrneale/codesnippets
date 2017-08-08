#include "DrawMethod_ToneMapper.h"
#include "..\VisualInterface.h"

DrawMethod_ToneMapper::DrawMethod_ToneMapper(LPDIRECT3DDEVICE9 device,SceneData *sd)
{
	UpdateLuminanceTimer.Duration			= 0.50f;
	UpdateCurrentLuminanceTimer.Duration	= 0.05f;
	CurrentLuminance						= 1.0f;
	VisibleLightSize						= 1.0f;
	LuminanceToReach						= 1.0f;
	MaximumLuminanceLevel					= 80.0f;
	MinimumLuminanceLevel					= 1.0f;
	LightAdjustSpeed						= 0.75f;
	MaxSceneLuminance						= 1.0f;
	MinSceneLuminance						= 0.0f;

	d3DDevice = device;
	sceneData = sd;

	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\ToneMap.fx",NULL,NULL,0,NULL,&ToneMapEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	Luminance = NULL;
	ScreenQuad.triangle(d3DDevice);

}

void DrawMethod_ToneMapper::RenderScene(DM_Output *outputRender)
{
	if (UpdateLuminanceTimer.Check())
	{
		GetLuminance(Luminance);
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
		if (MaxSceneLuminance>MaximumLuminanceLevel)
		{
			MaxSceneLuminance=MaximumLuminanceLevel;
		}
		if (MinSceneLuminance<MinimumLuminanceLevel)
		{
			MinSceneLuminance=MinimumLuminanceLevel;
		}
	}
	Render(outputRender);
}

void DrawMethod_ToneMapper::GetLuminance(LPDIRECT3DSURFACE9 Luminance)
{
	if (Luminance)
	{
		HRESULT hr = d3DDevice->GetRenderTargetData(Luminance,LPic);
		if (LPic)
		{
			float max = 0.0f;
			float min = 10000.0f;
			float avr = 0.0f;
			D3DSURFACE_DESC surfaceDesc;
			LPic->GetDesc(&surfaceDesc);
			D3DLOCKED_RECT locked;
			HRESULT hr=LPic->LockRect(&locked,0,D3DLOCK_READONLY);
			D3DXFLOAT16 *f = (D3DXFLOAT16*)locked.pBits;//D3DXFloat16To32Array()
			for (DWORD y=0;y<surfaceDesc.Height;y+=4)
			{
				for (DWORD x=0;x<surfaceDesc.Width;x+=4)
				{
					DWORD index=(x*4+(y*(6800)));

					// Blue
					float	b  = 0.06f * f[index+1];
							b += 0.67f * f[index+2];
							b += 0.27f * f[index+3];

					if (b>max)
					{
						max=b;
					}
					else if (b<min)
					{
						min=b;
					}
					avr+=b;
				}
			}
			avr /= ((surfaceDesc.Height/4.0f)*(surfaceDesc.Width/4.0f));
			LPic->UnlockRect();
			LuminanceToReach  = avr;
			MaxSceneLuminance = max;
			MinSceneLuminance = min;
		}
	}
}

void	DrawMethod_ToneMapper::Render(DM_Output* outputRender)
{
	HRESULT hr;
	d3DDevice->SetRenderTarget(0,outputRender->Render);
	d3DDevice->SetDepthStencilSurface(outputRender->Depth);

	ToneMapEffect->SetFloat("Luminance"		, CurrentLuminance);
	ToneMapEffect->SetFloat("MinLuminance"	, MinimumLuminanceLevel);
	ToneMapEffect->SetFloat("MaxLuminance"	, MaximumLuminanceLevel);
	ToneMapEffect->SetTexture("SceneTexture", ScenePic);

	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = ToneMapEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(ToneMapEffect->SetTechnique(hTech)))
		{
			ToneMapEffect->Begin(&Passes,0);
			ToneMapEffect->BeginPass(0);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			ToneMapEffect->EndPass();
			ToneMapEffect->End();
		}
	}
	hr = d3DDevice->EndScene();
}
