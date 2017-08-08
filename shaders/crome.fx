// GLOBAL VARIABLES
float4x4 WorldMatrix		: WORLD;
float4x4 ProjectionMatrix	: PROJECTION;
float4x4 ViewMatrix			: VIEW;

texture PositionTexture		: positionMap;
sampler2D PositionSample	= sampler_state{	Texture = <PositionTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture NormalTexture		: normalMap;
sampler2D NormalSample		= sampler_state{	Texture = <NormalTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture ColourTexture		: ColourMap;
sampler2D ColourSample		= sampler_state{	Texture = <ColourTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

float3	AmbientLight;

// STRUCTS	////////////////////////////////////////////
struct VSInput {	float4 Position	: POSITION;	};
struct VSOutput{    float4 Position	: POSITION;	float2 TexCoords	: TEXCOORD0; };
struct PSOutput{	float4 rt		: COLOR0;	};
//////////////////////////////////////////////////////////////////

float3 hemi[8] =
{
	float3(0.5f,0,0.5f),
	float3(sin(0.2f*3.14f),0,cos(0.2f*3.14f)),
	float3(sin(0.4f*3.14f),0,cos(0.4f*3.14f)),
	float3(sin(0.6f*3.14f),0,cos(0.6f*3.14f)),
	float3(sin(0.8f*3.14f),0,cos(0.8f*3.14f)),
	float3(sin(1.0f*3.14f),0,cos(1.0f*3.14f)),
	float3(sin(0.4f*3.14f),0,cos(0.4f*3.14f)),
	float3(sin(0.7f*3.14f),0,cos(0.7f*3.14f))		
};

float findocclusion(float3 pos,float3 normal,float2 uv, float radius)
{
	float ao=0;
	for (int a = 0; a<8; a++)
	{
		float3 hpos = hemi[a];
		hpos.x+(uv.x-0.5f)*0.001f;
		hpos.z+=(uv.y-0.5f)*0.001f;
		normalize(hpos);
		float3 checkpoint		= (hpos - 2 * dot(hpos, normal) * normal);
		uv.x+checkpoint.x;
		uv.y+checkpoint.z;
		
		float3 otherPosition	= mul(mul(tex2D(PositionSample	, checkpoint.xy),ViewMatrix),ProjectionMatrix).xyz;
		float t = abs(otherPosition.z-pos.z)*radius;
		ao+=t;	
	}
	return ao;
}
VSOutput AmbientVertexShader( VSInput input )
{
    VSOutput output		= (VSOutput)0;
    output.Position		= input.Position;
    output.TexCoords	= (input.Position.xy*0.5f)+0.5f;
    output.TexCoords.y	= 1.0f-output.TexCoords.y;
    return output;    
}

PSOutput AmbientPixelShader( VSOutput input ) 
{
	float3 screenNormal			= mul(mul(tex2D(NormalSample	, input.TexCoords),ViewMatrix),ProjectionMatrix).xyz;
	float4 screenposition		= mul(mul(tex2D(PositionSample	, input.TexCoords),ViewMatrix),ProjectionMatrix);
	float3 screenPosition		= screenposition.xyz;
	
	float	RadiusToCheck	= clamp(0.001f*(screenPosition.z/screenposition.w),0.00f,0.3f);
	
    PSOutput output		= (PSOutput)0;
    float3  colour		= tex2D(ColourSample,	input.TexCoords).xyz;
    float	occlusion	= findocclusion(screenPosition,screenNormal,input.TexCoords, RadiusToCheck);
	output.rt			= float4(colour*AmbientLight*occlusion,1.0f);
	return output;
}

// Techniques
technique Standard
{
    pass Ambient
    {   
        VertexShader = compile vs_3_0 AmbientVertexShader();
        PixelShader  = compile ps_3_0 AmbientPixelShader(); 
    }
}


