float4x4 WorldMatrix		: WORLD;
float4x4 ProjectionMatrix	: PROJECTION;
float4x4 ViewMatrix			: VIEW;
float3	 LightCol			: LIGHT;

texture LightTexture		: LightTextureMap;	sampler2D LightTextureSample	= sampler_state {	Texture = <LightTexture>;	MinFilter = Linear;    MagFilter = Linear;    MipFilter = Linear; };

struct LightVSInput
{
    float4 Position		: POSITION; 
    float2 TexCoords	: TEXCOORD0;
};

struct LightVSOutput
{
    float4 Position		: POSITION;
    float2 Uvs			: TEXCOORD0;
};

struct LightPSOutput
{
    float4 Color : COLOR0; 
};

LightVSOutput LightVS( LightVSInput input )
{
    LightVSOutput			output;
    output.Uvs				= input.TexCoords; 
   	float4 worldPosition	= mul( input.Position, WorldMatrix );
    float4 viewPosition		= mul( worldPosition, ViewMatrix );
    output.Position			= mul( viewPosition, ProjectionMatrix ); 
    return output;    
}

LightPSOutput LightPS( LightVSOutput input ) 
{ 
    LightPSOutput output;
	float4 scol	= tex2D(LightTextureSample, input.Uvs);
	output.Color = scol*float4(LightCol,1.0f)*2;
    return output;
}

texture PositionTexture		: positionMap;
sampler2D PositionSample	= sampler_state{	Texture = <PositionTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture NormalTexture		: normalMap;
sampler2D NormalSample		= sampler_state{	Texture = <NormalTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture ShadowTexture		: ShadowMap;
sampler2D ShadowSample		= sampler_state{	Texture = <ShadowTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;};

texture ColourTexture		: ColourMap;
sampler2D ColourSample		= sampler_state{	Texture = <ColourTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

float3	lightpos;
float3	lightcol;
float3	lightatt;
float3	conedir;
float   fov;

// STRUCTS	////////////////////////////////////////////
struct VSInput {	float4 Position	: POSITION;	};
struct VSOutput{    float4 Position	: POSITION;	float2 TexCoords	: TEXCOORD0; };
struct PSOutput{	float4 rt		: COLOR0;	};
//////////////////////////////////////////////////////////////////

VSOutput DiffuseVertexShader( VSInput input )
{
    VSOutput output		= (VSOutput)0;
    output.Position		= input.Position;
    output.TexCoords	= (input.Position.xy*0.5f)+0.5f;
    output.TexCoords.y	= 1.0f-output.TexCoords.y;
    return output;    
}

PSOutput DiffusePixelShader( VSOutput input ) 
{
    PSOutput output		= (PSOutput)0;
    float  shadow		= 0.0f;
	for (int a=0; a<5; a++)
	{
		float2 texco		=  float2(input.TexCoords.x + sin((a/5.0f)*3.14f)*0.005f,input.TexCoords.y + cos((a/5.0f)*3.14f)*0.005f);
		shadow				+= tex2D(ShadowSample, texco).x;
		texco				=  float2(input.TexCoords.x + sin((a/5.0f)*3.14f)*0.015f,input.TexCoords.y + cos((a/5.0f)*3.14f)*0.015f);
		shadow				+= tex2D(ShadowSample, texco).x*0.1f;
	}
	shadow/=5.5f;

 	output.rt				= float4(0,0,0,1);
    if (shadow!=0)
    {
	    float3 normal		= tex2D(NormalSample,	input.TexCoords).xyz;
		float3 position		= tex2D(PositionSample, input.TexCoords).xyz;
		float3 Colour		= tex2D(ColourSample, input.TexCoords).xyz;
		float  D			= distance(lightpos,position);
		float3 lightdir		= normalize(lightpos-position);
		float  lightAttenuation	= 1.0f / ( lightatt.x + lightatt.y*D );//+ lightatt.z*D * D);
		float  d			= clamp(dot( lightdir , normalize( normal ) ) , 0 , 1 );
		float  coneAttenuation	= -dot( conedir, lightdir );
		float t=0.0;
		if ((coneAttenuation)>cos(fov*0.5f))
		{
			t=pow(((coneAttenuation-(cos(fov*0.5f)))/(1-cos(fov*0.5f))),2);
		}
		output.rt			= float4(Colour*shadow*d*t*lightAttenuation*lightcol,1.0f);
	}
	return output;
}



technique Standard
{
    pass P0
    {   
        VertexShader	= compile vs_3_0 LightVS();
        PixelShader		= compile ps_3_0 LightPS(); 
    }
    pass Diffuse
    {   
        VertexShader = compile vs_3_0 DiffuseVertexShader();
        PixelShader  = compile ps_3_0 DiffusePixelShader(); 
    }
}
