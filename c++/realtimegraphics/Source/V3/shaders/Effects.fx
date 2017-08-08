// GLOBAL VARIABLES
float4x4 ProjectionMatrix	: PROJECTION;
float4x4 ViewMatrix			: VIEW;


texture TextureA		: visualEffectMap;
sampler2D SampleA		= sampler_state{	Texture = <TextureA>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

// STRUCTS	////////////////////////////////////////////
struct VSInput {	float4 Position	: POSITION; float3 Normal		: NORMAL;    float3 Tangent	: TANGENT;    float2 TexCoords : TEXCOORD0;	};
struct VSOutput{    float4 Position	: POSITION;	float2 TexCoords	: TEXCOORD0; float3 Colour	: TEXCOORD1;};
struct PSOutput{	float4 rt		: COLOR0;	};
//////////////////////////////////////////////////////////////////

VSOutput EffectVertexShader( VSInput input )
{
    VSOutput output			= (VSOutput)0;

    float4 worldPosition	= input.Position;
    float4 viewPosition		= mul( worldPosition, ViewMatrix );
    output.Position			= mul( viewPosition, ProjectionMatrix ); 
    output.TexCoords		= input.TexCoords;
    output.Colour			= input.Tangent;    
    return output;    
}

PSOutput EffectPixelShader( VSOutput input ) 
{
    PSOutput output		= (PSOutput)0;
    float4  col			= tex2D(SampleA,	input.TexCoords);
    col.x				*= input.Colour.x;
    col.y				*= input.Colour.y;
    col.z				*= input.Colour.z;    
	output.rt			= col;
	return output;
}

// Techniques
technique Standard
{
    pass Diffuse
    {   
        VertexShader = compile vs_3_0 EffectVertexShader();
        PixelShader  = compile ps_3_0 EffectPixelShader(); 
    }
}


