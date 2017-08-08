// GLOBAL VARIABLES
float4x4 WorldMatrix		: WORLD;
float4x4 ProjectionMatrix	: PROJECTION;
float4x4 ViewMatrix			: VIEW;

texture AlbedoTexture		: AlbedoMap;
sampler2D AlbedoSample		= sampler_state{	Texture = <AlbedoTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture NormalMapTexture	: NormalMapMap;
sampler2D NormalMapSample	= sampler_state{	Texture = <NormalMapTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture PositionTexture		: positionMap;
sampler2D PositionSample	= sampler_state{	Texture = <PositionTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture NormalTexture		: normalMap;
sampler2D NormalSample		= sampler_state{	Texture = <NormalTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

float3	BaseCol;
float3	lightpos;
float3	lightcol;
float3	lightatt;
float3	conedir;
float   fov;

// STRUCTS	////////////////////////////////////////////
struct VSInput
{
    float4 Position			: POSITION; 
    float3 Normal			: NORMAL;
    float3 Tangent			: TANGENT;
    float2 TexCoords		: TEXCOORD0;
};
struct VSOutput
{
    float4 Position			: POSITION;
    float4 WorldPosition	: TEXCOORD0;
    float3 Normal			: NORMAL;
    float3 Tangent			: TANGENT;
    float2 TexCoords		: TEXCOORD1;
};
struct PSOutput
{
  float4 rt[3] 				: COLOR0;
};
//////////////////////////////////////////////////////////////////

VSOutput dVertexShader( VSInput input )
{
    VSOutput output		= (VSOutput)0;
    float4x4 WorldMatrix2=WorldMatrix;
 	WorldMatrix2[3][0]=0;
	WorldMatrix2[3][1]=0;
	WorldMatrix2[3][2]=0;	
	float4 normal			= float4(input.Normal,1.0f);
	float4 newnormal        = mul( normal, WorldMatrix2 ); 
    // Normal	///////////////////////////////////////////////////
	output.Normal			= (newnormal/newnormal.w).xyz;

	///////////////////////////////////////////////////////////////
	// Position	///////////////////////////////////////////////////
    output.WorldPosition	= mul( input.Position, WorldMatrix );
    output.Position			= mul( mul( output.WorldPosition, ViewMatrix ), ProjectionMatrix );   
	// Texture Coordinates /////////////////////////////////////////////////////////////
    input.TexCoords.y		= 1.0f-input.TexCoords.y;
	output.TexCoords		= input.TexCoords;
	///////////////////////////////////////////////////////////////
	output.Tangent			= input.Tangent;
    return output;    
}

PSOutput dPixelShader( VSOutput input ) 
{ 
    PSOutput output	= (PSOutput)0;
    output.rt[0]	= input.WorldPosition;

	float4 Normaladjust = tex2D(NormalMapSample, input.TexCoords);
	input.Normal	= (input.Normal + Normaladjust.xyz)/2;

    output.rt[1]	= float4(input.Normal,0.0f);
	output.rt[2]	= float4(BaseCol,1.0f);//tex2D(AlbedoSample, input.TexCoords)*
    return output;
}

// STRUCTS	////////////////////////////////////////////
struct LightVSInput
{
    float4 Position			: POSITION; 
};
struct LightVSOutput
{
    float4 Position			: POSITION;
    float2 Position2		: TEXCOORD0;
};
struct LightPSOutput
{
  float4 rt 				: COLOR0;
};
//////////////////////////////////////////////////////////////////

LightVSOutput lVertexShader( LightVSInput input )
{
    LightVSOutput output	= (LightVSOutput)0;
    
    output.Position			= input.Position;
    output.Position2		= (input.Position.xy*0.5f)+0.5f;
    output.Position2.y		= 1.0f-output.Position2.y;
    return output;    
}

LightPSOutput lPixelShader( LightVSOutput input ) 
{
    LightPSOutput output	= (LightPSOutput)0;
    
   	float3 normal			= tex2D(NormalSample,	input.Position2).xyz;
	float3 position			= tex2D(PositionSample, input.Position2).xyz;
	float4 colour			= tex2D(AlbedoSample,	input.Position2);
	float  D				= distance(lightpos,position);
	float3 lightdir			= normalize(lightpos-position);
	float  lightAttenuation	= 1.0f / ( lightatt.x + lightatt.y*D + lightatt.z*D * lightatt.z*D);
	float  d				= dot( lightdir , normalize(normal));
	float  coneAttenuation	= -dot( conedir, lightdir );
	float t=0.0;//1-abs(coneAttenuation);
	if ((coneAttenuation)>cos(fov*0.5f))
	{
		t=pow(((coneAttenuation-(cos(fov*0.5f)))/(1-cos(fov*0.5f))),2);
	}
	output.rt				= colour*t*float4(lightcol*d*lightAttenuation,1.0f);
	
	return output;
}

// Techniques
technique Standard
{
    pass P0
    {   
        VertexShader = compile vs_3_0 dVertexShader();
        PixelShader  = compile ps_3_0 dPixelShader(); 
    }
    pass P1
    {   
        VertexShader = compile vs_3_0 lVertexShader();
        PixelShader  = compile ps_3_0 lPixelShader(); 
    }
}


