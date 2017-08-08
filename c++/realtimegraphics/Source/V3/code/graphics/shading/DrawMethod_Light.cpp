#include "DrawMethod_Light.h"
#include "..\VisualInterface.h"

DrawMethod_Light::DrawMethod_Light(LPDIRECT3DDEVICE9 device,SceneData *sd)
{
	d3DDevice=device;
	sceneData=sd;
	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\light.fx",NULL,NULL,0,NULL,&LightEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
	ScreenQuad.triangle(device);
}

void DrawMethod_Light::RenderScene(DM_Output* out)
{
	if (out && LightEffect)
	{
		d3DDevice->SetRenderTarget(0,out->Render);
		d3DDevice->SetDepthStencilSurface(out->Depth);
		RenderLightPass(out);
	}
}

void DrawMethod_Light::RenderLightPass(DM_Output* out)
{
	//HRESULT hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
	//d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//d3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//d3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//d3DDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	//d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//hr = LightEffect->SetTexture("PositionTexture",PositionPass);
	//hr = LightEffect->SetTexture("NormalTexture",NormalPass);
	//hr = LightEffect->SetTexture("AlbedoTexture",ColourPass);
	//for (int lightnum=0; lightnum < (int)sceneData->Lights.size(); lightnum++)
	//{
	//	if (SUCCEEDED(d3DDevice->BeginScene()))
	//	{
	//		D3DXHANDLE hTech=NULL;
	//		UINT Passes;
	//		hTech = DeferredEffect->GetTechniqueByName("Standard");
	//		if (SUCCEEDED(DeferredEffect->SetTechnique(hTech)))
	//		{
	//			hr = DeferredEffect->Begin(&Passes,0);
	//			hr = DeferredEffect->SetFloatArray("lightpos",&sceneData->Lights[lightnum]->Position.x,3);
	//			D3DXVECTOR3 lightdir = sceneData->Lights[lightnum]->Interest - sceneData->Lights[lightnum]->Position;
	//			D3DXVec3Normalize(&lightdir,&lightdir);
	//			hr = DeferredEffect->SetFloatArray("conedir",&lightdir.x,3);
	//			hr = DeferredEffect->SetFloat("fov",sceneData->Lights[lightnum]->FOV);
	//			hr = DeferredEffect->SetFloatArray("lightatt",&sceneData->Lights[lightnum]->Attenuation.x,3);
	//			hr = DeferredEffect->SetFloatArray("lightcol",&sceneData->Lights[lightnum]->Col.R,3);
	//			hr = DeferredEffect->BeginPass(1);
	//			hr = d3DDevice->SetStreamSource(0,ScreenQuad._vb,0,44 );
	//			hr = d3DDevice->SetIndices(ScreenQuad._ib);
	//			hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ScreenQuad.NumofVerts,0,ScreenQuad.PrimitiveCount);
	//			hr = DeferredEffect->EndPass();
	//			hr = DeferredEffect->End();
	//		}
	//		hr = d3DDevice->EndScene();
	//	}
	//}
	//d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
