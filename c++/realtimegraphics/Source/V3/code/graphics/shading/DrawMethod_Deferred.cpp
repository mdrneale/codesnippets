#include "DrawMethod_Deferred.h"
#include "..\VisualInterface.h"
#include "..\..\input\InputInterface.h"

enum BlendTypes
{
	BT_Multiply = 0,
	BT_Add		= 1
};

DrawMethod_Deferred::DrawMethod_Deferred(LPDIRECT3DDEVICE9 device,SceneData *sd) 
{
	d3DDevice=device;
	sceneData=sd;
	SRT		= SRT_ToneMap;
	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\Version2\\Ubershader.fx",NULL,NULL,0,NULL,&UberEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	HRESULT hr;
	T.Duration		= 0.05f;

	LightTextureID = VisualInterface::I()->LoadTexture("textures\\light.tga");
	//////////////////////////////////////////////////////////////////////////
	// G buffer
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &PositionTexture, NULL);
	hr	= PositionTexture->GetSurfaceLevel(0,&PositionSurface);
	//
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &NormalTexture, NULL);
	hr	= NormalTexture->GetSurfaceLevel(0,&NormalSurface);
	//
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A8R8G8B8),		D3DPOOL_DEFAULT, &ColourTexture, NULL);
	hr	= ColourTexture->GetSurfaceLevel(0,&ColourSurface);
	//
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &ColourTexture, NULL);
	hr	= ColourTexture->GetSurfaceLevel(0,&ColourSurface);
	//
	static const unsigned int swidth = WIDTH;
	static const unsigned int sheight = WIDTH;//2048;
	hr	= d3DDevice->CreateTexture(	swidth, sheight,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_R16F),	D3DPOOL_DEFAULT, &ShadowMapTexture, NULL);
	hr	= ShadowMapTexture->GetSurfaceLevel(0,&ShadowMapSurface);
	//
	hr	= d3DDevice->CreateTexture(	swidth, sheight,	0, D3DUSAGE_DEPTHSTENCIL, D3DFMT_D16,	D3DPOOL_DEFAULT, &ShadowMapDepthTexture, NULL);
	hr	= ShadowMapDepthTexture->GetSurfaceLevel(0,&ShadowMapDepthSurface);
	//
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_R16F),	D3DPOOL_DEFAULT, &TempShadowTexture, NULL);
	hr	= TempShadowTexture->GetSurfaceLevel(0,&TempShadowSurface);
	// 
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &SwitchTexture1, NULL);
	hr	= SwitchTexture1->GetSurfaceLevel(0,&SwitchSurface1);
	//
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &SwitchTexture2, NULL);
	hr	= SwitchTexture2->GetSurfaceLevel(0,&SwitchSurface2);
	//
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &SwitchTexture3, NULL);
	hr	= SwitchTexture3->GetSurfaceLevel(0,&SwitchSurface3);
	// Tone mapping
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_DYNAMIC, (D3DFMT_A16B16G16R16F),	D3DPOOL_SYSTEMMEM, &LuminanceTexture, NULL);
	hr	= LuminanceTexture->GetSurfaceLevel(0,&LuminanceSurface);
	//////////////////////////////////////////////////////////////////////////
	// Tone Mapping stuff
	UpdateLuminanceTimer.Duration			= 0.50f;
	UpdateCurrentLuminanceTimer.Duration	= 0.05f;
	CurrentLuminance						= 1.0f;
	VisibleLightSize						= 1.0f;
	LuminanceToReach						= 1.0f;
	MaximumLuminanceLevel					= 2.5f;
	MinimumLuminanceLevel					= 0.20f;
	LightAdjustSpeed						= 0.75f;
	MaxSceneLuminance						= 1.0f;
	MinSceneLuminance						= 0.0f;
	//////////////////////////////////////////////////////////////////////////
	ScreenQuad.triangle(device);
}

DrawMethod_Deferred::~DrawMethod_Deferred()
{
	PositionTexture->Release();
	PositionSurface->Release();
	NormalTexture->Release();
	NormalSurface->Release();
	ColourTexture->Release();
	ColourSurface->Release();
	ShadowMapTexture->Release();
	ShadowMapSurface->Release();
	ShadowMapDepthTexture->Release();
	ShadowMapDepthSurface->Release();
	TempShadowTexture->Release();
	TempShadowSurface->Release();
	SwitchTexture1->Release();
	SwitchSurface1->Release();
	SwitchTexture2->Release();
	SwitchSurface2->Release();
	SwitchTexture3->Release();
	SwitchSurface3->Release();
	LuminanceTexture->Release();
	LuminanceSurface->Release();
}

void DrawMethod_Deferred::RenderScene(DM_Output* out)
{
	static bool sr=false;
	int issr = (int)inputinterface::I()->getaction("switch render");
	if (issr && !sr)
	{
		SRT = (SelectRenderTarget)((SRT + 1)%SRT_NumberOfSRTs);
		sr = true;
	}
	else if (!issr)
	{
		sr = false;
	}
	if (out && UberEffect)
	{
		output = out;
		CreateGBuffer();
		AddAmbient();
		ShadeScene();
		ToneMap();
		TVNoise();
	}
}

void DrawMethod_Deferred::CreateGBuffer()
{
	d3DDevice->SetDepthStencilSurface(output->Depth);
	d3DDevice->SetRenderTarget(0,PositionSurface);
	d3DDevice->SetRenderTarget(1,NormalSurface);
	d3DDevice->SetRenderTarget(2,ColourSurface);

	if (SRT==SRT_Position)
	{
		d3DDevice->SetRenderTarget(0,output->Render);
	}
	else if (SRT == SRT_Normal)
	{
		d3DDevice->SetRenderTarget(1,output->Render);
	}
	else if (SRT == SRT_Colour)
	{
		d3DDevice->SetRenderTarget(2,output->Render);
	}

	HRESULT hr;
	hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		SetVeiwMatrix(*sceneData->Cameras[sceneData->CurrentCamera],UberEffect);
		SetProjectionMatrix(sceneData->Cameras[sceneData->CurrentCamera]->FOV,0.1f,1000.0f,UberEffect);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = UberEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(UberEffect->SetTechnique(hTech)))
		{
			UberEffect->Begin(&Passes,0);
			for (int modnum=0; modnum < (int)sceneData->Models.size(); modnum++)
			{
				SetWorldMatrix(*sceneData->Models[modnum],UberEffect);
				UberEffect->SetFloatArray			("BaseCol",sceneData->Models[modnum]->BaseCol,3);
				if (sceneData->Models[modnum]->normalMapID!=-1)
				{
					UberEffect->SetTexture				("NormalTexture",sceneData->Textures[sceneData->Models[modnum]->normalMapID]);
				}
				UberEffect->BeginPass(0);
				hr = d3DDevice->SetStreamSource		(0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->_vb,0,44 );
				hr = d3DDevice->SetIndices			(sceneData->Meshes[sceneData->Models[modnum]->MeshID]->_ib);
				hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->NumofVerts,0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->PrimitiveCount);
				UberEffect->EndPass();
			}
			UberEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderTarget(0,output->Render);
	d3DDevice->SetRenderTarget(1,NULL);
	d3DDevice->SetRenderTarget(2,NULL);
	d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET ,D3DCOLOR_XRGB(0,0,0),1.0f,0);
}

void DrawMethod_Deferred::AddAmbient()
{
	if (SRT==SRT_Ambient)
	{
		d3DDevice->SetRenderTarget(0,output->Render);
		d3DDevice->SetRenderTarget(1,SwitchSurface2);
	}
	else
	{
		d3DDevice->SetRenderTarget(0,SwitchSurface1);
		d3DDevice->SetRenderTarget(1,SwitchSurface2);
	}

	d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET ,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);

	HRESULT hr;
	hr = UberEffect->SetTexture("PositionTexture",PositionTexture);
	hr = UberEffect->SetTexture("NormalTexture",NormalTexture);
	hr = UberEffect->SetTexture("ColourTexture",ColourTexture);
	hr = UberEffect->SetFloatArray("BaseCol",&sceneData->AmbientColour.R,3);

	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = UberEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(UberEffect->SetTechnique(hTech)))
		{
			SetVeiwMatrix(*sceneData->Cameras[sceneData->CurrentCamera],UberEffect);
			SetProjectionMatrix(sceneData->Cameras[sceneData->CurrentCamera]->FOV,0.1f,1000.0f,UberEffect);
			hr = UberEffect->Begin(&Passes,0);
			hr = UberEffect->BeginPass(1);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			hr = UberEffect->EndPass();
			hr = UberEffect->End();
		}
		hr = d3DDevice->EndScene();
	}

	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	d3DDevice->SetRenderTarget(1,NULL);

}

void DrawMethod_Deferred::ShadeScene()
{
	for (unsigned int a = 0; a<sceneData->Lights.size(); a++)
	{
		if (GetVisability(sceneData->Lights[a]))
		{
			CreateShadowMap(sceneData->Lights[a]);
			RenderShadowMapToScene(sceneData->Lights[a]);
			BlurForSoftShadows();
			ShadeTheScene(sceneData->Lights[a]);
		}
	}
	d3DDevice->SetDepthStencilSurface(output->Depth);
	DrawShadeSpecificObjects();
	DrawLightBulbs();
}

bool DrawMethod_Deferred::GetVisability(Light* l)
{
	return true;
}

void DrawMethod_Deferred::CreateShadowMap(Light* l)
{
	if (SRT==SRT_ShadowMap)
	{
		d3DDevice->SetRenderTarget(0,output->Render);
	}
	else
	{
		d3DDevice->SetRenderTarget(0,ShadowMapSurface);
	}
	d3DDevice->SetDepthStencilSurface(ShadowMapDepthSurface);
	HRESULT hr;
	hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		SetVeiwMatrix(l,UberEffect);
		SetProjectionMatrix(l->FOV,1.0f,500.0f,UberEffect,1.0f);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = UberEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(UberEffect->SetTechnique(hTech)))
		{
			UberEffect->Begin(&Passes,0);
			for (int modnum=0; modnum < (int)sceneData->Models.size(); modnum++)
			{
				SetWorldMatrix(*sceneData->Models[modnum],UberEffect);
				UberEffect->BeginPass(2);
				hr = d3DDevice->SetStreamSource(0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->_vb,0,44 );
				hr = d3DDevice->SetIndices(sceneData->Meshes[sceneData->Models[modnum]->MeshID]->_ib);
				hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->NumofVerts,0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->PrimitiveCount);
				UberEffect->EndPass();
			}
			UberEffect->End();
		}
	}
	hr = d3DDevice->EndScene();
}

void DrawMethod_Deferred::RenderShadowMapToScene(Light* l)
{
	if (SRT==SRT_ShadowMapToScene)
	{
		d3DDevice->SetRenderTarget(0,output->Render);
	}
	else
	{
		d3DDevice->SetRenderTarget(0,TempShadowSurface);
	}
	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	SetVeiwMatrix(l,UberEffect);
	SetProjectionMatrix(l->FOV, 1.0f, 500.0f,UberEffect,1.0f);
	HRESULT hr;
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		UberEffect->SetTexture("PositionTexture",PositionTexture);
		UberEffect->SetTexture("ColourTexture",ShadowMapTexture);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = UberEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(UberEffect->SetTechnique(hTech)))
		{
			UberEffect->Begin(&Passes,0);
			UberEffect->BeginPass(3);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			UberEffect->EndPass();
		}
		UberEffect->End();
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void DrawMethod_Deferred::BlurForSoftShadows()
{
	d3DDevice->SetRenderTarget(0,SwitchSurface3);
	HRESULT hr;
	hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);

	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		UberEffect->SetTexture("NormalTexture",TempShadowTexture);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = UberEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(UberEffect->SetTechnique(hTech)))
		{
			UberEffect->Begin(&Passes,0);
			UberEffect->BeginPass(4);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			UberEffect->EndPass();
		}
		UberEffect->End();
		hr = d3DDevice->EndScene();
	}

	if (SRT==SRT_BlurShadowMapToScene)
	{
		d3DDevice->SetRenderTarget(0,output->Render);
	}
	else
	{
		d3DDevice->SetRenderTarget(0,TempShadowSurface);
	}

	hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		UberEffect->SetTexture("NormalTexture",SwitchTexture3);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = UberEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(UberEffect->SetTechnique(hTech)))
		{
			UberEffect->Begin(&Passes,0);
			UberEffect->BeginPass(5);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			UberEffect->EndPass();
		}
		UberEffect->End();
		hr = d3DDevice->EndScene();
	}

	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void DrawMethod_Deferred::ShadeTheScene(Light* l)
{
	if (SRT==SRT_ShadeTheScene)
	{
		d3DDevice->SetRenderTarget(0,output->Render);
		d3DDevice->SetRenderTarget(1,SwitchSurface2);
	}
	else
	{
		d3DDevice->SetRenderTarget(0,SwitchSurface1);
		d3DDevice->SetRenderTarget(1,SwitchSurface2);
	}
	HRESULT hr;
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	d3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	d3DDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	hr = UberEffect->SetTexture("PositionTexture",PositionTexture);
	hr = UberEffect->SetTexture("NormalTexture",NormalTexture);
	hr = UberEffect->SetTexture("ShadowTexture",TempShadowTexture);
	hr = UberEffect->SetTexture("ColourTexture",ColourTexture);
	hr = UberEffect->SetFloatArray("lightpos",&l->Position.x,3);
	hr = UberEffect->SetFloatArray("lightatt",&l->Attenuation.x,3);
	hr = UberEffect->SetFloatArray("BaseCol",&l->Col.R,3);
	D3DXVECTOR3 lightdir = l->Interest - l->Position;
	D3DXVec3Normalize(&lightdir,&lightdir);
	hr = UberEffect->SetFloatArray("conedir",&lightdir.x,3);
	hr = UberEffect->SetFloat("fov",l->FOV);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = UberEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(UberEffect->SetTechnique(hTech)))
		{
			hr = UberEffect->Begin(&Passes,0);
			hr = UberEffect->BeginPass(6);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			hr = UberEffect->EndPass();
			hr = UberEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	d3DDevice->SetRenderTarget(1,NULL);
}

void DrawMethod_Deferred::DrawShadeSpecificObjects()
{
	d3DDevice->SetRenderTarget(0,SwitchSurface1);

	for (unsigned int a = 0; a<sceneData->Models.size(); a++)
	{
		if (sceneData->Models[a]->EffectID!=-1)
		{
			int effid=sceneData->Models[a]->EffectID;
			if (sceneData->effects[effid]->Effect)
			{
				LPD3DXEFFECT objectsEffect = sceneData->effects[effid]->Effect;
				if (sceneData->effects[effid]->NeedsPosMap && PositionTexture)
				{
					objectsEffect->SetTexture("PositionTexture",PositionTexture);
				}
				if (sceneData->effects[effid]->NeedsColMap && SwitchTexture2)
				{
					objectsEffect->SetTexture("ColourTexture",SwitchTexture2);
				}
				if (sceneData->effects[effid]->NeedsNormMap	&& NormalTexture)
				{
					objectsEffect->SetTexture("NormalTexture",NormalTexture);
				}
				if (sceneData->effects[effid]->BlendType==BT_Multiply)
				{
				}
				d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				DrawAShadeSpecificObject(objectsEffect,sceneData->Models[a]);
				d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			}
		}
	}
}

void DrawMethod_Deferred::DrawAShadeSpecificObject(LPD3DXEFFECT Effect,Model* model)
{
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	HRESULT hr;
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

void DrawMethod_Deferred::DrawLightBulbs()
{
	if (SRT==SRT_DrawLightBulbs)
	{
		d3DDevice->SetRenderTarget(0,output->Render);
	}
	else
	{
		d3DDevice->SetRenderTarget(0,SwitchSurface1);
	}

	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	d3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	HRESULT hr;// = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),0.0f,0);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		SetVeiwMatrix(*sceneData->Cameras[sceneData->CurrentCamera],UberEffect);
		SetProjectionMatrix(sceneData->Cameras[sceneData->CurrentCamera]->FOV,0.1f,1000.0f,UberEffect);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = UberEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(UberEffect->SetTechnique(hTech)))
		{
			UberEffect->Begin(&Passes,0);
			for (unsigned int a = 0; a<sceneData->Lights.size(); a++)
			{
				UberEffect->SetTexture("NormalTexture",sceneData->Textures[LightTextureID]);
				SetScreenFacingLight(*sceneData->Lights[a],UberEffect);
				D3DXVECTOR3 lcol(&sceneData->Lights[a]->Col.R);
				D3DXVec3Normalize(&lcol,&lcol);
				UberEffect->SetFloatArray("BaseCol",&sceneData->Lights[a]->Col.R,3);	//				will change for average check later
				UberEffect->BeginPass(7);
				hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
				hr = d3DDevice->SetIndices(ScreenQuad._ib);
				hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
				UberEffect->EndPass();
			}
			UberEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void DrawMethod_Deferred::SetScreenFacingLight(const Light &l,LPD3DXEFFECT E)
{
	D3DXMATRIX	MatrixView , MatrixBill;	
	D3DXMatrixLookAtLH( &MatrixView, &sceneData->Cameras[sceneData->CurrentCamera]->Position, &sceneData->Cameras[sceneData->CurrentCamera]->Interest, &sceneData->Cameras[sceneData->CurrentCamera]->Up );
	MatrixView._41 = 0.0f;
	MatrixView._42 = 0.0f;
	MatrixView._43 = 0.0f;
	D3DXMatrixInverse( &MatrixBill, NULL, &MatrixView );
	MatrixBill._41 = l.Position.x;
	MatrixBill._42 = l.Position.y;
	MatrixBill._43 = l.Position.z;
	E->SetMatrix("WorldMatrix",&MatrixBill);
}

void DrawMethod_Deferred::ToneMap()
{
	if (UpdateLuminanceTimer.Check())
	{
		GetLuminance(SwitchSurface1);
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
	DrawToneMap();
}

void DrawMethod_Deferred::GetLuminance(LPDIRECT3DSURFACE9 lum)
{
	if (lum)
	{
		HRESULT hr = d3DDevice->GetRenderTargetData(lum,LuminanceSurface);
		if (LuminanceSurface)
		{
			float max = 0.0f;
			float min = 10000.0f;
			float avr = 0.0f;
			D3DSURFACE_DESC surfaceDesc;
			LuminanceSurface->GetDesc(&surfaceDesc);
			D3DLOCKED_RECT locked;
			HRESULT hr=LuminanceSurface->LockRect(&locked,0,D3DLOCK_READONLY);
			D3DXFLOAT16 *f = (D3DXFLOAT16*)locked.pBits;//D3DXFloat16To32Array()
			for (DWORD y=0;y<surfaceDesc.Height;y+=4)
			{
				for (DWORD x=0;x<surfaceDesc.Width;x+=4)
				{
					DWORD index=(x*4+(y*(surfaceDesc.Width*4)));

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
			LuminanceSurface->UnlockRect();
			LuminanceToReach  = avr;
			MaxSceneLuminance = max;
			MinSceneLuminance = min;
		}
	}
}

void DrawMethod_Deferred::DrawToneMap()
{
	HRESULT hr;
	d3DDevice->SetRenderTarget(0,SwitchSurface2);
	d3DDevice->SetRenderTarget(1,SwitchSurface3);
	d3DDevice->SetDepthStencilSurface(output->Depth);
	d3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	UberEffect->SetFloat("fov"			, CurrentLuminance);
//	UberEffect->SetFloat("MinLuminance"		, MinimumLuminanceLevel);
//	UberEffect->SetFloat("MaxLuminance"		, MaximumLuminanceLevel);
	UberEffect->SetTexture("ColourTexture"	, SwitchTexture1);

	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = UberEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(UberEffect->SetTechnique(hTech)))
		{
			UberEffect->Begin(&Passes,0);
			UberEffect->BeginPass(8);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			UberEffect->EndPass();
			UberEffect->End();
		}
	}
	hr = d3DDevice->EndScene();
	d3DDevice->SetRenderTarget(0,SwitchSurface1);
	d3DDevice->SetRenderTarget(1,NULL);

	hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);

	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		UberEffect->SetTexture("NormalTexture",SwitchTexture3);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = UberEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(UberEffect->SetTechnique(hTech)))
		{
			UberEffect->Begin(&Passes,0);
			UberEffect->BeginPass(9);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			UberEffect->EndPass();
		}
		UberEffect->End();
		hr = d3DDevice->EndScene();
	}

	d3DDevice->SetRenderTarget(0,SwitchSurface3);//SwitchSurface3);

	hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		UberEffect->SetTexture("NormalTexture",SwitchTexture1);
		UberEffect->SetTexture("PositionTexture",SwitchTexture2);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = UberEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(UberEffect->SetTechnique(hTech)))
		{
			UberEffect->Begin(&Passes,0);
			UberEffect->BeginPass(10);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			UberEffect->EndPass();
		}
		UberEffect->End();
		hr = d3DDevice->EndScene();
	}

	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void DrawMethod_Deferred::TVNoise()
{
	static int tvStaticTextureID	= VisualInterface::I()->LoadTexture("textures\\tv.tga");
	UberEffect->SetTexture("TvStatic",sceneData->Textures[tvStaticTextureID]);
	static int currentFrame	= 0;
	static float tvStaticWeight	= 0.025f;

	if (T.Check())
	{
		currentFrame=(currentFrame+1)%15;
	}
	UberEffect->SetInt		("Time"			,currentFrame);
	UberEffect->SetFloat	("Weight"		,tvStaticWeight);
	UberEffect->SetTexture	("SceneTexture",SwitchTexture3);
	d3DDevice->SetRenderTarget(0,output->Render);
	d3DDevice->SetDepthStencilSurface(output->Depth);
	HRESULT hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(1,1,1),1.0f,0);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech;
		UINT Passes;
		hTech = UberEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(UberEffect->SetTechnique(hTech)))
		{
			UberEffect->Begin(&Passes,0);
			UberEffect->BeginPass(11);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			UberEffect->EndPass();
			UberEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
}
