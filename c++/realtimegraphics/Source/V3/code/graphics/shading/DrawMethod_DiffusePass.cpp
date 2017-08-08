#include "DrawMethod_DiffusePass.h"
#include "..\VisualInterface.h"

DrawMethod_DiffusePass::DrawMethod_DiffusePass(LPDIRECT3DDEVICE9 device,SceneData *sd)
{
	d3DDevice = device;
	sceneData = sd;

	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\diffuse.fx",NULL,NULL,0,NULL,&DiffuseEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\shadowSecondpass.fx",NULL,NULL,0,NULL,&ShadowSecondPassEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\blur.fx",NULL,NULL,0,NULL,&BlurEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\shadow.fx",NULL,NULL,0,NULL,&ShadowEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}

	ScreenQuad.triangle(d3DDevice);
	PositionPass	= NULL;
	NormalPass		= NULL;
	HRESULT hr;
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	1, D3DUSAGE_RENDERTARGET, ((_D3DFORMAT)113),	D3DPOOL_DEFAULT, &ShadowPass, NULL);
	hr	= ShadowPass->GetSurfaceLevel(0,&ShadowSurface);
	hr	= d3DDevice->CreateTexture(	WIDTH, HEIGHT,	1, D3DUSAGE_RENDERTARGET, ((_D3DFORMAT)113),	D3DPOOL_DEFAULT, &FinalShadowPass, NULL);
	hr	= FinalShadowPass->GetSurfaceLevel(0,&FinalShadowSurface);
}

void DrawMethod_DiffusePass::RenderScene(DM_Output* out)
{
	if (out && PositionPass && NormalPass && DiffuseEffect)
	{
		d3DDevice->SetRenderTarget(0,out->Render);
		d3DDevice->SetRenderTarget(0,out->Depth);
		d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
		d3DDevice->SetRenderTarget(0,ShadowSurface);
		d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
		for (unsigned int a=0; a<sceneData->Lights.size(); ++a)
		{
			d3DDevice->SetRenderTarget(0,FinalShadowSurface);
			d3DDevice->SetRenderTarget(0,out->Depth);
			RenderShadowMap(a);
			d3DDevice->SetRenderTarget(0,ShadowSurface);
			d3DDevice->SetRenderTarget(0,out->Depth);
			RenderShadows(a);
			d3DDevice->SetRenderTarget(0,out->Render);
			d3DDevice->SetRenderTarget(0,out->Depth);
			RenderBlur();
			d3DDevice->SetRenderTarget(0,out->Render);
			d3DDevice->SetRenderTarget(0,out->Depth);
			Render(a);
		}
//		d3DDevice->SetRenderTarget(0,out->Render);
//		d3DDevice->SetRenderTarget(0,out->Depth);
//		RenderSecondShadowPass();
	}
}

void DrawMethod_DiffusePass::Render(int a)
{
	HRESULT hr;
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	d3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	d3DDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	hr = DiffuseEffect->SetTexture("PositionTexture",PositionPass);
	hr = DiffuseEffect->SetTexture("NormalTexture",NormalPass);
	hr = DiffuseEffect->SetTexture("ShadowTexture",ShadowPass);
	hr = DiffuseEffect->SetFloatArray("lightpos",&sceneData->Lights[a]->Position.x,3);
	hr = DiffuseEffect->SetFloatArray("lightatt",&sceneData->Lights[a]->Attenuation.x,3);
	hr = DiffuseEffect->SetFloatArray("lightcol",&sceneData->Lights[a]->Col.R,3);
	D3DXVECTOR3 lightdir = sceneData->Lights[a]->Interest - sceneData->Lights[a]->Position;
	D3DXVec3Normalize(&lightdir,&lightdir);
	hr = DiffuseEffect->SetFloatArray("conedir",&lightdir.x,3);
	hr = DiffuseEffect->SetFloat("fov",sceneData->Lights[a]->FOV);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = DiffuseEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(DiffuseEffect->SetTechnique(hTech)))
		{
			hr = DiffuseEffect->Begin(&Passes,0);
			hr = DiffuseEffect->BeginPass(0);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			hr = DiffuseEffect->EndPass();
			hr = DiffuseEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void DrawMethod_DiffusePass::RenderShadowMap(int a)
{
	HRESULT hr;
	hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		SetVeiwMatrix		(*sceneData->Lights[a],ShadowEffect);
		SetProjectionMatrix	(sceneData->Lights[a]->FOV,0.1f,1000.0f,ShadowEffect);
		ShadowEffect->SetFloat("Zmax",5.0f);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = ShadowEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(ShadowEffect->SetTechnique(hTech)))
		{
			ShadowEffect->Begin(&Passes,0);
			for (int modnum=0; modnum < (int)sceneData->Models.size(); modnum++)
			{
				SetWorldMatrix(*sceneData->Models[modnum],ShadowEffect);
				ShadowEffect->BeginPass(0);
				hr = d3DDevice->SetStreamSource(0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->_vb,0,44 );
				hr = d3DDevice->SetIndices(sceneData->Meshes[sceneData->Models[modnum]->MeshID]->_ib);
				hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->NumofVerts,0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->PrimitiveCount);
				ShadowEffect->EndPass();
			}
			ShadowEffect->End();
		}
	}
	hr = d3DDevice->EndScene();
}

void DrawMethod_DiffusePass::RenderShadows(int a)
{
	SetVeiwMatrix(*sceneData->Lights[a],ShadowEffect);
	SetProjectionMatrix(sceneData->Lights[a]->FOV, 0.1f, 1000.0f,ShadowEffect);
	ShadowEffect->SetFloatArray("lightcol",&sceneData->Lights[a]->Col.R,3);
	ShadowEffect->SetInt("NumofLights",sceneData->Lights.size());
	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	d3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	d3DDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	HRESULT hr;
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		ShadowEffect->SetTexture("PositionTexture",PositionPass);
		ShadowEffect->SetTexture("ShadowTexture",FinalShadowPass);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = ShadowEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(ShadowEffect->SetTechnique(hTech)))
		{
			ShadowEffect->Begin(&Passes,0);
			ShadowEffect->BeginPass(1);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			ShadowEffect->EndPass();
		}
		ShadowEffect->End();
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void DrawMethod_DiffusePass::RenderBlur()
{
	HRESULT hr;
	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	hr = BlurEffect->SetTexture("ImageTexture",ShadowPass);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = BlurEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(BlurEffect->SetTechnique(hTech)))
		{
			hr = BlurEffect->Begin(&Passes,0);
			hr = BlurEffect->BeginPass(0);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			hr = BlurEffect->EndPass();
			hr = BlurEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
}

void DrawMethod_DiffusePass::RenderSecondShadowPass()
{
	HRESULT hr;
	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	hr = ShadowSecondPassEffect->SetTexture("ShadowTexture",ShadowPass);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = ShadowSecondPassEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(ShadowSecondPassEffect->SetTechnique(hTech)))
		{
			hr = ShadowSecondPassEffect->Begin(&Passes,0);
			hr = ShadowSecondPassEffect->BeginPass(0);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			hr = ShadowSecondPassEffect->EndPass();
			hr = ShadowSecondPassEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
}