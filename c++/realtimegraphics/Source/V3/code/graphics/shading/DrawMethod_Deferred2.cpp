#include "DrawMethod_Deferred2.h"
#include "..\VisualInterface.h"
#include "DrawMethod_DebugOutput.h"

DrawMethod_deferred2::DrawMethod_deferred2(LPDIRECT3DDEVICE9 device,SceneData *sd): TV(device,sd),Ap(device,sd),Sp(device,sd),EP(device,sd),SSp(device,sd),TM(device,sd)
{
	d3DDevice = device;
	sceneData = sd;
	
	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\deffered2.fx",NULL,NULL,0,NULL,&DeferredEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}

	LightTextureID = VisualInterface::I()->LoadTexture("textures\\light.tga");

	PositionPass	= NULL;
	PositionSurface = NULL;

	NormalPass		= NULL;
	NormalSurface	= NULL;

	ColourPass		= NULL;
	ColourSurface	= NULL;

	ScreenQuad.triangle(d3DDevice);

	HRESULT hr;
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &PositionPass, NULL);
	hr	= PositionPass->GetSurfaceLevel(0,&PositionSurface);

	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &NormalPass, NULL);
	hr	= NormalPass->GetSurfaceLevel(0,&NormalSurface);

	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A8R8G8B8),	D3DPOOL_DEFAULT, &ColourPass, NULL);
	hr	= ColourPass->GetSurfaceLevel(0,&ColourSurface);

	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &TempSwitchPass1, NULL);
	hr	= TempSwitchPass1->GetSurfaceLevel(0,&TempSwitchSurface1);

	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &TempSwitchPass2, NULL);
	hr	= TempSwitchPass2->GetSurfaceLevel(0,&TempSwitchSurface2);

	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_A16B16G16R16F),	D3DPOOL_DEFAULT, &SwitchPass, NULL);
	hr	= SwitchPass->GetSurfaceLevel(0,&SwitchSurface);

	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_R16F),	D3DPOOL_DEFAULT, &LowResTempPass, NULL);
	hr	= LowResTempPass->GetSurfaceLevel(0,&LowResTempSurface);

	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	0, D3DUSAGE_DYNAMIC, (D3DFMT_A16B16G16R16F),	D3DPOOL_SYSTEMMEM, &mipmaplum, NULL);
	hr	= mipmaplum->GetSurfaceLevel(0,&mipmaplumSurface);
	Ap.SetPositionTexture(PositionPass);
	Ap.SetNormalTexture(NormalPass);
	Ap.SetColourTexture(ColourPass);
	Sp.SetPositionTexture(PositionPass);
	Sp.SetNormalTexture(NormalPass);
	Sp.SetColourTexture(ColourPass);
	Sp.SetLowResSurface(LowResTempSurface,LowResTempPass);
	Sp.SetLightTextureID(LightTextureID);
	SSp.SetPositionTexture(PositionPass);
	SSp.SetNormalTexture(NormalPass);
	Sp.Blur.SetTempBlureSurface(TempSwitchSurface2,TempSwitchPass2);

}

void DrawMethod_deferred2::RenderScene(DM_Output* out)
{
	if (out)
	{
		// create the G buffer
		d3DDevice->SetDepthStencilSurface(out->Depth);
		d3DDevice->SetRenderTarget(0,PositionSurface);
		d3DDevice->SetRenderTarget(1,NormalSurface);
		d3DDevice->SetRenderTarget(2,ColourSurface);
		Render();
		// clear the output render
		d3DDevice->SetRenderTarget(0,out->Render);
		d3DDevice->SetRenderTarget(1,NULL);
		d3DDevice->SetRenderTarget(2,NULL);
		d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
		d3DDevice->SetRenderTarget(0,LowResTempSurface);
		d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);

		DM_Output temp;
		static bool pswitch = true;
		if (pswitch)
		{
			SSp.SetColourTexture(SwitchPass);
			temp.Render = TempSwitchSurface1;
			TM.SetSceneTexture(SwitchPass);
			TM.SetLuminanceSurface(SwitchSurface,mipmaplumSurface);
		}
		else
		{
			SSp.SetColourTexture(TempSwitchPass1);
			temp.Render = SwitchSurface;
			TM.SetSceneTexture(TempSwitchPass1);
			TM.SetLuminanceSurface(TempSwitchSurface1,mipmaplumSurface);
		}
		temp.Depth	= out->Depth;
		Sp.RenderScene(&temp);
		Ap.RenderScene(&temp);
	//	EP.RenderScene(&temp);
		SSp.RenderScene(&temp);
		temp.Render = TempSwitchSurface2;
		TV.preprocessRender = TempSwitchPass2;

		TM.RenderScene(&temp);
		TV.RenderScene(out);
		if (pswitch)
		{
			d3DDevice->SetRenderTarget(0,SwitchSurface);
			d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
		}
		else
		{
			d3DDevice->SetRenderTarget(0,TempSwitchSurface1);
			d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
		}

		pswitch=!pswitch;
	}
}

void DrawMethod_deferred2::Render()
{
	HRESULT hr;
	hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		SetVeiwMatrix(*sceneData->Cameras[sceneData->CurrentCamera],DeferredEffect);
		SetProjectionMatrix(sceneData->Cameras[sceneData->CurrentCamera]->FOV,0.1f,1000.0f,DeferredEffect);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = DeferredEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(DeferredEffect->SetTechnique(hTech)))
		{
			DeferredEffect->Begin(&Passes,0);
			for (int modnum=0; modnum < (int)sceneData->Models.size(); modnum++)
			{
				SetWorldMatrix(*sceneData->Models[modnum],DeferredEffect);
				DeferredEffect->SetFloatArray("BaseCol",sceneData->Models[modnum]->BaseCol,3);
//				DeferredEffect->SetTexture("AlbedoTexture",sceneData->Textures[sceneData->Models[modnum]->diffuseMapID]);
				DeferredEffect->SetTexture("NormalTexture",sceneData->Textures[sceneData->Models[modnum]->normalMapID]);
				DeferredEffect->BeginPass(0);
				hr = d3DDevice->SetStreamSource(0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->_vb,0,44 );
				hr = d3DDevice->SetIndices(sceneData->Meshes[sceneData->Models[modnum]->MeshID]->_ib);
				hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->NumofVerts,0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->PrimitiveCount);
				DeferredEffect->EndPass();
			}
			DeferredEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
}