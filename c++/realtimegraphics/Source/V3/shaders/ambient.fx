// GLOBAL VARIABLES
float4x4 WorldMatrix		: WORLD;
float4x4 ProjectionMatrix	: PROJECTION;
float4x4 ViewMatrix			: VIEW;

texture PositionTexture		: positionMap;
sampler2D PositionSample	= sampler_state{	Texture = <PositionTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear; AddressU = Mirror; AddressV = Mirror; };

texture NormalTexture		: normalMap;
sampler2D NormalSample		= sampler_state{	Texture = <NormalTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	AddressU = Mirror; AddressV = Mirror;};

texture ColourTexture		: ColourMap;
sampler2D ColourSample		= sampler_state{	Texture = <ColourTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

float3	AmbientLight;

// STRUCTS	////////////////////////////////////////////
struct VSInput {	float4 Position	: POSITION;	};
struct VSOutput{    float4 Position	: POSITION;	float2 TexCoords	: TEXCOORD0; };
struct PSOutput{	float4 rt		: COLOR0;	};
//////////////////////////////////////////////////////////////////

float3 pSphere[16] = 
{
	float3(0.53812504, 0.18565957, -0.43192),		float3(0.13790712, 0.24864247, 0.44301823),
	float3(0.33715037, 0.56794053, -0.005789503),	float3(-0.6999805, -0.04511441, -0.0019965635),
	float3(0.06896307, -0.15983082, -0.85477847),	float3(0.056099437, 0.006954967, -0.1843352),
	float3(-0.014653638, 0.14027752, 0.0762037),	float3(0.010019933, -0.1924225, -0.034443386),
	float3(-0.35775623, -0.5301969, -0.43581226),	float3(-0.3169221, 0.106360726, 0.015860917),
	float3(0.010350345, -0.58698344, 0.0046293875),	float3(-0.08972908, -0.49408212, 0.3287904),
	float3(0.7119986, -0.0154690035, -0.09183723),	float3(-0.053382345, 0.059675813, -0.5411899),
	float3(0.035267662, -0.063188605, 0.54602677),	float3(-0.47761092, 0.2847911, -0.0271716)
};

float findocclusion(float3 pos,float3 normal,float2 uv, float radius)
{
	float falloff= 0.025f;
	float ao=16.0f;
	for (int a=0; a<16; a++)
	{
		float3 SecondPos = pSphere[a];//sign(dot(pSphere[a],normal))*pSphere[a];
		SecondPos*=0.2f*clamp( ((1-(radius))*(1-(radius))),0,1);
		//SecondPos.xy*=(1.0f-abs(SecondPos.z));
		SecondPos.xy+=uv;
		float4 temp				= tex2D(PositionSample	, SecondPos.xy);
		float3 SecondPosition	= temp.xyz/temp.w;
		
		float dist = distance(pos,SecondPosition)/falloff;
		float3 normaltopoint = normalize(SecondPosition-pos);
		float dif = dot(normal,normaltopoint);
		if (dif>0.15f)
		{
			ao-=(0.15f+(1.0f*0.85f));//clamp(dot(normal,-normalize(SecondPosition-pos)*(dist/falloff)),0.0f,1.0f);
		}
	}
	return ao/16;
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
    PSOutput output		= (PSOutput)0;

	float3 screenNormal		= tex2D(NormalSample	, input.TexCoords).xyz;//,ViewMatrix),ProjectionMatrix).xyz;
	float4 temp				= tex2D(PositionSample	, input.TexCoords);//,ViewMatrix),ProjectionMatrix);
	float3 screenPosition	= temp.xyz;///temp.w;
	temp					= mul(mul(temp,ViewMatrix),ProjectionMatrix);
	float  ratio			= (temp.w/400.0f);
	
    float3  colour		= tex2D(ColourSample,	input.TexCoords).xyz;
    float	occlusion	= findocclusion(screenPosition,screenNormal,input.TexCoords, ratio);
    
	output.rt			= float4(colour*AmbientLight*occlusion,1.0f);
	//output.rt.r=	ratio;
	
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


