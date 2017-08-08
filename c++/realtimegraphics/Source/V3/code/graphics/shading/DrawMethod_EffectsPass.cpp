#include "DrawMethod_EffectsPass.h"
#include "..\VisualInterface.h"
#include "..\..\VisualEffects\EffectsSystem.h"

DrawMethod_EffectsPass::DrawMethod_EffectsPass(LPDIRECT3DDEVICE9 device,SceneData *sd)
{
	d3DDevice = device;
	sceneData = sd;

	LPD3DXBUFFER	code = NULL;
	if (E_FAIL == D3DXCreateEffectFromFile(d3DDevice,"shaders\\Effects.fx",NULL,NULL,0,NULL,&EffectEffect,&code))
	{
		char * Emsg=(char*)code->GetBufferPointer();
		OutputDebugString(Emsg);
		code->Release();
	}
}

void DrawMethod_EffectsPass::RenderScene(DM_Output* out)
{
	if (out && EffectEffect)
	{
		d3DDevice->SetRenderTarget(0,out->Render);
		d3DDevice->SetDepthStencilSurface(out->Depth);
		Render();
	}
}
void DrawMethod_EffectsPass::GetRenderInfo()
{
}

void DrawMethod_EffectsPass::Render()
{
	RenderInfo *ri = EffectsSystem::I()->GetRenderInfo();
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	d3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	HRESULT hr;
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		Camera* c;
		VisualInterface::I()->GetCamera(&c);
		SetVeiwMatrix(*c,EffectEffect);
		SetProjectionMatrix(c->FOV,0.1f,1000.0f,EffectEffect);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = EffectEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(EffectEffect->SetTechnique(hTech)))
		{
			EffectEffect->Begin(&Passes,0);
			hr = d3DDevice->SetStreamSource(0,ri->VertexBuffer,0,44 );
			hr = d3DDevice->SetIndices(ri->IndexBuffer);
			EffectEffect->SetTexture("TextureA",sceneData->Textures[ri->FirstTextureID]);
			if (ri->TextureIDs[0])
			{
				EffectEffect->BeginPass(0);
				hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,ri->TextureIDs[0]*4,0,ri->TextureIDs[0]*2);
				EffectEffect->EndPass();
			}
			for (int a=1; a<NumberOfTextures; a++)
			{
				if (ri->TextureIDs[a])
				{
					EffectEffect->SetTexture("TextureA",sceneData->Textures[ri->FirstTextureID+(a)]);
					EffectEffect->BeginPass(0);
					hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,(ri->TextureIDs[a]-ri->TextureIDs[a-1])*6,ri->TextureIDs[a-1]*6,(ri->TextureIDs[a]-ri->TextureIDs[a-1])*2);
					EffectEffect->EndPass();
				}
			}
			EffectEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
	d3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	d3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
