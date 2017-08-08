// GLOBAL VARIABLES
float4x4 WorldMatrix		: WORLD;
float4x4 ProjectionMatrix	: PROJECTION;
float4x4 ViewMatrix			: VIEW;

texture PositionTexture		: positionMap;
sampler2D PositionSample	= sampler_state{	Texture = <PositionTexture>;	AddressU = Clamp; AddressV = Clamp;};

texture NormalTexture		: normalMap;
sampler2D NormalSample		= sampler_state{	Texture = <NormalTexture>;	AddressU = Clamp; AddressV = Clamp;};

texture ColourTexture		: ColourMap;
sampler2D ColourSample		= sampler_state{	Texture = <ColourTexture>;	AddressU = Wrap; AddressV = Clamp;};

float3	AmbientLight;

// STRUCTS	////////////////////////////////////////////
struct VSInput {	float4 Position	: POSITION;	};
struct VSOutput{    float4 Position	: POSITION;	float2 TexCoords	: TEXCOORD0; };
struct PSOutput{	float4 rt		: COLOR0;	};
//////////////////////////////////////////////////////////////////

VSOutput ChromeVertexShader( VSInput input )
{
    VSOutput output		= (VSOutput)0;
    output.Position		= mul(mul(mul(input.Position, WorldMatrix),ViewMatrix),ProjectionMatrix);
    output.TexCoords	= ((output.Position.xy/output.Position.w)*0.5f)+float2(0.5f,0.5f);
    output.TexCoords.y	= 1.0f-output.TexCoords.y;
    return output;    
}

PSOutput ChromePixelShader( VSOutput input ) 
{
	float3 screenNormal			= mul(mul(tex2D(NormalSample	, input.TexCoords),ViewMatrix),ProjectionMatrix).xyz;
	float a		=	abs(screenNormal.z);
	screenNormal.y = 1.0f - screenNormal.y;
	float2 Uv	= input.TexCoords + (screenNormal.xy*(a));
//	Uv.y=1.0f-Uv.y;
    PSOutput output		= (PSOutput)0;
    float3  colour1		= tex2D(ColourSample,	input.TexCoords	).xyz;
    float3  colour2		= tex2D(ColourSample,	Uv				).xyz;
	output.rt			= float4((colour1*a)+(colour2*(1.0f-a)),1.0f);
	return output;
}

// Techniques
technique Standard
{
    pass Ambient
    {   
        VertexShader = compile vs_3_0 ChromeVertexShader();
        PixelShader  = compile ps_3_0 ChromePixelShader(); 
    }
}


