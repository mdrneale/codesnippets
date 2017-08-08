#include "DrawMethod_Shadows.h"
#include "..\VisualInterface.h"
#include "DrawMethod_DebugOutput.h"

DrawMethod_Shadows::DrawMethod_Shadows(LPDIRECT3DDEVICE9 device,SceneData *sd): Blur(device,sd)
{
	d3DDevice = device;
	sceneData = sd;

	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\Version2\\Ubershader.fx",NULL,NULL,0,NULL,&UberEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}

	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\shadows.fx",NULL,NULL,0,NULL,&ShadowEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\shadefirstlightpass.fx",NULL,NULL,0,NULL,&ShadingEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}

	ShadowMapTexture	= NULL;
	ShadowMapSurface	= NULL;

	ScreenQuad.triangle(d3DDevice);

	HRESULT hr;
	static const unsigned int swidth = HEIGHT;
	static const unsigned int sheight = HEIGHT;//2048;
	hr	= d3DDevice->CreateTexture(	swidth, sheight,	0, D3DUSAGE_RENDERTARGET, (D3DFMT_R16F),	D3DPOOL_DEFAULT, &ShadowMapTexture, NULL);
	hr	= ShadowMapTexture->GetSurfaceLevel(0,&ShadowMapSurface);

	hr	= d3DDevice->CreateTexture(	swidth, sheight,	0, D3DUSAGE_DEPTHSTENCIL, D3DFMT_D16,	D3DPOOL_DEFAULT, &ShadowMapDepthTexture, NULL);
	hr	= ShadowMapDepthTexture->GetSurfaceLevel(0,&ShadowMapDepthSurface);
	
	PositionPass	= NULL;
	LowResSurface	= NULL;
	NormalPass		= NULL;
	ColourPass		= NULL;
	LightTextureID	= -1;

}

void DrawMethod_Shadows::RenderScene(DM_Output* out)
{
	if (LowResSurface && PositionPass && out)
	{
		d3DDevice->SetRenderTarget(0,out->Render);
		d3DDevice->SetDepthStencilSurface(out->Depth);
		d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET ,D3DCOLOR_XRGB(0,0,0),1.0f,0);
		for (unsigned int a = 0; a<sceneData->Lights.size(); a++)
		{
			if (GetVisability(a))
			{
				//	Create the ShadowMap
				CreateAShadowMap(a);
				//	Render the shadow to a smaller buffer
				RenderTheShadow(a);
//				Blur.SetBlurTexture(LowResTexture);
//				Blur.RenderScene(out);
				d3DDevice->SetRenderTarget(0,out->Render);
				d3DDevice->SetDepthStencilSurface(out->Depth);
				ShadeScene(a);
				//	draw reflective / semi-transparent objects
				DrawLight(a);
			}
		}
	}
}

void DrawMethod_Shadows::CreateAShadowMap	(unsigned int LightSource)
{
	HRESULT hr;
	d3DDevice->SetRenderTarget(0,ShadowMapSurface);
	d3DDevice->SetDepthStencilSurface(ShadowMapDepthSurface);
	hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		SetVeiwMatrix(*sceneData->Lights[LightSource],ShadowEffect);
		SetProjectionMatrix(sceneData->Lights[LightSource]->FOV,1.0f,500.0f,ShadowEffect,1.0f);
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

void DrawMethod_Shadows::RenderTheShadow	(unsigned int LightSource)
{
	d3DDevice->SetRenderTarget(0,LowResSurface);
//	d3DDevice->SetDepthStencilSurface(out->Depth);

	SetVeiwMatrix(*sceneData->Lights[LightSource],ShadowEffect);
	SetProjectionMatrix(sceneData->Lights[LightSource]->FOV, 1.0f, 500.0f,ShadowEffect,1.0f);

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
		ShadowEffect->SetTexture("ShadowTexture",ShadowMapTexture);
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

bool DrawMethod_Shadows::GetVisability		(unsigned int LightSource)
{
	return 1;
}

void DrawMethod_Shadows::ShadeScene			(unsigned int LightSource)
{
	HRESULT hr;
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	d3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	d3DDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	hr = ShadingEffect->SetTexture("PositionTexture",PositionPass);
	hr = ShadingEffect->SetTexture("NormalTexture",NormalPass);
	hr = ShadingEffect->SetTexture("ShadowTexture",LowResTexture);
	hr = ShadingEffect->SetTexture("ColourTexture",ColourPass);
	hr = ShadingEffect->SetFloatArray("lightpos",&sceneData->Lights[LightSource]->Position.x,3);
	hr = ShadingEffect->SetFloatArray("lightatt",&sceneData->Lights[LightSource]->Attenuation.x,3);
	hr = ShadingEffect->SetFloatArray("lightcol",&sceneData->Lights[LightSource]->Col.R,3);
	D3DXVECTOR3 lightdir = sceneData->Lights[LightSource]->Interest - sceneData->Lights[LightSource]->Position;
	D3DXVec3Normalize(&lightdir,&lightdir);
	hr = ShadingEffect->SetFloatArray("conedir",&lightdir.x,3);
	hr = ShadingEffect->SetFloat("fov",sceneData->Lights[LightSource]->FOV);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = ShadingEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(ShadingEffect->SetTechnique(hTech)))
		{
			hr = ShadingEffect->Begin(&Passes,0);
			hr = ShadingEffect->BeginPass(1);
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			hr = ShadingEffect->EndPass();
			hr = ShadingEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


//	hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	d3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
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
			D3DXMATRIX	LightView;
			D3DXMatrixLookAtLH( &LightView, &sceneData->Lights[LightSource]->Position, &sceneData->Lights[LightSource]->Interest, &sceneData->Lights[LightSource]->Up );
			D3DXMATRIX	WorldOrInversLightViewInThisCase;
			float		Diterminant;
			D3DXMatrixInverse(&WorldOrInversLightViewInThisCase,&Diterminant,&LightView);
			D3DXMATRIX	matScale;
			D3DXMatrixScaling(&matScale,sceneData->Lights[LightSource]->FOV*D3DX_PI*0.5f*sceneData->Lights[LightSource]->Col.B,sceneData->Lights[LightSource]->FOV*D3DX_PI*0.5f*sceneData->Lights[LightSource]->Col.B,sceneData->Lights[LightSource]->Col.B);

			UberEffect->SetMatrix("WorldMatrix",&(matScale*WorldOrInversLightViewInThisCase));
			UberEffect->BeginPass(0);
			hr = d3DDevice->SetStreamSource(0,sceneData->Meshes[sceneData->Lights[LightSource]->LightBox]->_vb,0,44 );
			hr = d3DDevice->SetIndices(sceneData->Meshes[sceneData->Lights[LightSource]->LightBox]->_ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,sceneData->Meshes[sceneData->Lights[LightSource]->LightBox]->NumofVerts,0,sceneData->Meshes[sceneData->Lights[LightSource]->LightBox]->PrimitiveCount);
			UberEffect->EndPass();
			UberEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

void DrawMethod_Shadows::DrawLight			(unsigned int LightSource)
{
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	d3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	HRESULT hr;// = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),0.0f,0);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		SetVeiwMatrix(*sceneData->Cameras[sceneData->CurrentCamera],ShadingEffect);
		SetProjectionMatrix(sceneData->Cameras[sceneData->CurrentCamera]->FOV,0.1f,1000.0f,ShadingEffect);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = ShadingEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(ShadingEffect->SetTechnique(hTech)))
		{
			ShadingEffect->Begin(&Passes,0);
			ShadingEffect->SetTexture("LightTexture",sceneData->Textures[LightTextureID]);
			ShadingEffect->BeginPass(0);
			SetScreenFacingLight(*sceneData->Lights[LightSource]);
			D3DXVECTOR3 lcol(&sceneData->Lights[LightSource]->Col.R);
			D3DXVec3Normalize(&lcol,&lcol);
			ShadingEffect->SetFloatArray("LightCol",&sceneData->Lights[LightSource]->Col.R,3);	//				will change for average check later
			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
			hr = d3DDevice->SetIndices(ScreenQuad._ib);
			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
			ShadingEffect->EndPass();
			ShadingEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void DrawMethod_Shadows::SetScreenFacingLight(const Light &l)
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
	ShadingEffect->SetMatrix("WorldMatrix",&MatrixBill);
}
