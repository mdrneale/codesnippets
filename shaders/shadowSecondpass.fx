texture PositionTexture		: positionMap;
sampler2D PositionSample	= sampler_state{	Texture = <PositionTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture NormalTexture		: normalMap;
sampler2D NormalSample		= sampler_state{	Texture = <NormalTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture ShadowTexture		: ShadowMap;
sampler2D ShadowSample		= sampler_state{	Texture = <ShadowTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	AddressU = Mirror; AddressV = Mirror;};

// STRUCTS	////////////////////////////////////////////
struct VSInput2	{	float4 Position	: POSITION;	};
struct VSOutput2{	float4 Position	: POSITION;	float2 texcoords	: TEXCOORD0;	};
struct PSOutput2{	float4 rt	: COLOR0;	};
//////////////////////////////////////////////////////////////////

VSOutput2 aVertexShader( VSInput2 input )
{
    VSOutput2 output		= (VSOutput2)0;
	///////////////////////////////////////////////////////////////
	// Position	///////////////////////////////////////////////////
    output.Position		= input.Position;
    output.texcoords	= (input.Position.xy*0.5f)+0.5f;
    output.texcoords.y	= 1.0f-output.texcoords.y;
    return output;    
}

PSOutput2 aPixelShader( VSOutput2 input ) 
{ 
    PSOutput2 output		= (PSOutput2)0;
    float3 position			= tex2D( PositionSample , input.texcoords ).xyz;
    float3 normal			= tex2D( NormalSample	, input.texcoords ).xyz;
    float4 colour			= float4(0,0,0,1);
    for (int a = 0; a<2; a++)
    {
	    for (int b = 0; b<2; b++)
		{
			float2 uv				= float2((input.texcoords.x/1.0f)+(a/2.0f),(input.texcoords.y/1.0f)+(b/2.0f));
			float3 samplePosition	= tex2D( PositionSample , uv ).xyz;
			float3 sampleNormal		= tex2D( NormalSample	, uv ).xyz;
			float4 sampleColour		= tex2D( ShadowSample	, uv );
			float dist				= distance(samplePosition,position);
			float3 direction		= normalize(samplePosition-position);
			colour					+=sampleColour;//*clamp(dot(direction,normal),0.0,1.0f);
		}
    }
    colour/=4.0f;
    output.rt = colour;
    return output;
}

// Techniques
technique Standard
{
    pass p0
    {   
        VertexShader = compile vs_3_0 aVertexShader();
        PixelShader  = compile ps_3_0 aPixelShader(); 
    }
}


