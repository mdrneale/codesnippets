#include "DrawMethod_Material.h"
#include "..\VisualInterface.h"
#include "..\..\input\InputInterface.h"

DrawMethod_Material::DrawMethod_Material(LPDIRECT3DDEVICE9 device,SceneData *sd) 
{
	d3DDevice=device;
	sceneData=sd;
	MaterialEffect = NULL;
}

void DrawMethod_Material::RenderScene(DM_Output* out)
{
	if (out && MaterialEffect)
	{
		d3DDevice->SetRenderTarget(0,out->Render);
		d3DDevice->SetDepthStencilSurface(out->Depth);
		RenderMaterialPass();
	}
}

void DrawMethod_Material::RenderMaterialPass()
{
	HRESULT hr = d3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(10,10,10),1.0f,0);
	if (SUCCEEDED(d3DDevice->BeginScene()))
	{
		SetVeiwMatrix(*sceneData->Cameras[sceneData->CurrentCamera]);
		SetProjectionMatrix(D3DX_PI / 4.0f,0.1f,1000.0f);
		D3DXHANDLE hTech=NULL;
		UINT Passes;
		hTech = MaterialEffect->GetTechniqueByName("Standard");
		if (SUCCEEDED(MaterialEffect->SetTechnique(hTech)))
		{
			MaterialEffect->Begin(&Passes,0);
			for (unsigned int modnum=0; modnum < sceneData->Models.size(); modnum++)
			{
				MaterialEffect->BeginPass(0);
				MaterialEffect->SetTexture("diffuseTexture",sceneData->Textures[sceneData->Models[modnum]->diffuseMapID]);
				SetWorldMatrix(*sceneData->Models[modnum]);
				hr = d3DDevice->SetStreamSource(0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->_vb,0,44 );
				hr = d3DDevice->SetIndices(sceneData->Meshes[sceneData->Models[modnum]->MeshID]->_ib);
				hr = d3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->NumofVerts,0,sceneData->Meshes[sceneData->Models[modnum]->MeshID]->PrimitiveCount);
				MaterialEffect->EndPass();
			}
			MaterialEffect->End();
		}
		hr = d3DDevice->EndScene();
	}
}

void DrawMethod_Material::SetWorldMatrix			( const Model &mod				)
{
	D3DXMATRIX	matRotationX,
				matRotationY,
				matRotationZ,
				matScale,
				matTranslation; 
	/////////////////////////////////////////////////////////////
	// Set World Matrix
	D3DXMatrixScaling(&matScale,mod.Scaling.x,mod.Scaling.y,mod.Scaling.z);
	D3DXMatrixRotationX(&matRotationX,mod.Rotation.x);
	D3DXMatrixRotationY(&matRotationY,mod.Rotation.y);
	D3DXMatrixRotationZ(&matRotationZ,mod.Rotation.z);
	D3DXMatrixTranslation(&matTranslation,mod.Position.x,mod.Position.y,mod.Position.z);
	D3DXMATRIX MatrixWorld = (	matScale * matRotationX * matRotationY * matRotationZ * matTranslation);
	// Give it to the effect
	MaterialEffect->SetMatrix("WorldMatrix",&MatrixWorld);
}

void DrawMethod_Material::SetVeiwMatrix			( const Camera &cam				)
{
	D3DXMATRIX	MatrixView;	
	D3DXMatrixLookAtLH( &MatrixView, &cam.Position, &cam.Interest, &cam.Up );
	MaterialEffect->SetMatrix("ViewMatrix",&MatrixView);
}

void DrawMethod_Material::SetVeiwMatrix			( const Light &l				)
{
	D3DXMATRIX	MatrixView;	
	D3DXMatrixLookAtLH( &MatrixView, &l.Position, &l.Interest, &l.Up );
	MaterialEffect->SetMatrix("ViewMatrix",&MatrixView);
}

void DrawMethod_Material::SetProjectionMatrix	( float FOV, float nearPlane, float farPlane		)
{
	D3DXMATRIX	matProjection;									//projection matrix (NEW)
	const float	AspectRatio=(float)WIDTH / (float)HEIGHT;		//view port ratio (NEW)

	D3DXMatrixPerspectiveFovLH(	&matProjection, FOV, AspectRatio, nearPlane, farPlane);
	MaterialEffect->SetMatrix("ProjectionMatrix",&matProjection);
}