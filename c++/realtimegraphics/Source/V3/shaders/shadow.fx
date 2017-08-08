// GLOBAL VARIABLES
float4x4 WorldMatrix		: WORLD;
float4x4 ProjectionMatrix	: PROJECTION;
float4x4 ViewMatrix			: VIEW;

texture PositionTexture		: positionMap;
sampler2D PositionSample	= sampler_state{	Texture = <PositionTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture ShadowTexture		: ShadowMap;
sampler2D ShadowSample		= sampler_state{	Texture = <ShadowTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	AddressU = Clamp; AddressV = Clamp;};

float   Zmax;
float3  lightcol;
int		NumofLights;
// STRUCTS	////////////////////////////////////////////
struct VSInput	{	float4 Position	: POSITION; };
struct VSOutput	{	float4 Position	: POSITION;	float4 Pos	: TEXCOORD0;};
struct PSOutput	{	float4 rt	: COLOR0;	};
//////////////////////////////////////////////////////////////////

VSOutput sVertexShader( VSInput input )
{
    VSOutput output		= (VSOutput)0;
	///////////////////////////////////////////////////////////////
	// Position	///////////////////////////////////////////////////
    output.Pos	= mul( input.Position, WorldMatrix );
    output.Pos	= output.Position = mul( mul( output.Pos, ViewMatrix ), ProjectionMatrix );   
    return output;    
}

PSOutput sPixelShader( VSOutput input ) 
{ 
    PSOutput output	= (PSOutput)0;
    output.rt		= input.Pos.z/Zmax;
    return output;
}

// STRUCTS	////////////////////////////////////////////
struct VSInput2	{	float4 Position	: POSITION;	};
struct VSOutput2{	float4 Position	: POSITION;	float2 texcoords	: TEXCOORD0;	};
struct PSOutput2{	float4 rt	: COLOR0;	};
//////////////////////////////////////////////////////////////////

VSOutput2 s2VertexShader( VSInput2 input )
{
    VSOutput2 output		= (VSOutput2)0;
	///////////////////////////////////////////////////////////////
	// Position	///////////////////////////////////////////////////
    output.Position		= input.Position;
    output.texcoords	= (input.Position.xy*0.5f)+0.5f;
    output.texcoords.y	= 1.0f-output.texcoords.y;
    return output;    
}

PSOutput2 s2PixelShader( VSOutput2 input ) 
{ 
    PSOutput2 output		= (PSOutput2)0;
    float4 position			= tex2D( PositionSample , input.texcoords );
    float4 PosInLightSpace	= mul( mul( tex2D( PositionSample , input.texcoords ) , ViewMatrix ) , ProjectionMatrix );
    float2 uv				= (PosInLightSpace.xy / PosInLightSpace.w) * 0.5f + float2(0.5f,0.5f);
    uv.y					= 1-uv.y;
    float storez			= PosInLightSpace.z / Zmax;
    if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f || storez < 0.0f || (position.x == 0.0f && position.y == 0.0f && position.z == 0.0f))
    {
		output.rt = 0.0f;
	    return output;		
    } 
    float depthfromlight	= tex2D(ShadowSample,uv).z;
    if (storez-0.05f <= depthfromlight)
    {
		output.rt = float4(normalize(lightcol)/NumofLights,1.0f);
	    return output;
	}
    float sample1	= tex2D(ShadowSample,uv+float2(-0.05f,-0.05f)).z;
    float sample2	= tex2D(ShadowSample,uv+float2( 0.05f,-0.05f)).z;
    float sample3	= tex2D(ShadowSample,uv+float2( 0.05f, 0.05f)).z;
    float sample4	= tex2D(ShadowSample,uv+float2(-0.05f, 0.05f)).z;
	float avr		= sample1*sample2*sample3*sample4*depthfromlight/5.0f;
	output.rt = float4(lightcol,1.0f);
    return output;
}

// Techniques
technique Standard
{
    pass RenderShadowMap
    {   
        VertexShader = compile vs_3_0 sVertexShader();
        PixelShader  = compile ps_3_0 sPixelShader(); 
    }
    pass RenderShadows
    {   
        VertexShader = compile vs_3_0 s2VertexShader();
        PixelShader  = compile ps_3_0 s2PixelShader(); 
    }
}


