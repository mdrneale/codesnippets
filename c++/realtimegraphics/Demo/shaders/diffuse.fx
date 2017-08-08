// GLOBAL VARIABLES
texture PositionTexture		: positionMap;
sampler2D PositionSample	= sampler_state{	Texture = <PositionTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture NormalTexture		: normalMap;
sampler2D NormalSample		= sampler_state{	Texture = <NormalTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture ShadowTexture		: ShadowMap;
sampler2D ShadowSample		= sampler_state{	Texture = <ShadowTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

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
    float  shadow		= tex2D(ShadowSample,	input.TexCoords).x;
	output.rt				= float4(0,0,0,1);
    if (shadow!=0)
    {
	    float3 normal		= tex2D(NormalSample,	input.TexCoords).xyz;
		float3 position		= tex2D(PositionSample, input.TexCoords).xyz;
		float  D			= distance(lightpos,position);
		float3 lightdir		= normalize(lightpos-position);
		float  lightAttenuation	= 1.0f / ( lightatt.x + lightatt.y*D + lightatt.z*D * lightatt.z*D);
		float  d			= clamp(dot( lightdir , normalize( normal ) ) , 0 , 1 );
		float  coneAttenuation	= -dot( conedir, lightdir );
		float t=0.0;
		if ((coneAttenuation)>cos(fov*0.5f))
		{
			t=pow(((coneAttenuation-(cos(fov*0.5f)))/(1-cos(fov*0.5f))),2);
		}
		output.rt			= float4(shadow*d*t*lightAttenuation*lightcol,1.0f);
	}
	return output;
}

// Techniques
technique Standard
{
    pass Diffuse
    {   
        VertexShader = compile vs_3_0 DiffuseVertexShader();
        PixelShader  = compile ps_3_0 DiffusePixelShader(); 
    }
}


