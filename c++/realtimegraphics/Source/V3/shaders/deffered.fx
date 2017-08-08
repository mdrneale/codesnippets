// GLOBAL VARIABLES
float4x4 WorldMatrix		: WORLD;
float4x4 ProjectionMatrix	: PROJECTION;
float4x4 ViewMatrix			: VIEW;

texture diffuseTexture		: DiffuseMap;
sampler2D diffuseSample = sampler_state{	Texture = <diffuseTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture SceneTexture		: SceneMap;
sampler2D SceneSample =	sampler_state{	Texture	  = <SceneTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

// VERTEX STRUCTS

struct VSInput
{
    float4 Position		: POSITION; 
    float3 Normal		: NORMAL;
    float3 Tangent		: TANGENT;
    float2 TexCoords	: TEXCOORD0;
};

struct MaterialVertexShaderOutput
{
    float4 Position		: POSITION;
    float4 Position2	: TEXCOORD0;
    float2 Uv			: TEXCOORD1;
};

struct VSInput2
{
    float4 Position		: POSITION; 
};

// PIXEL STRUCTS

struct MaterialPixelShaderOutput
{
    float4 Color		: COLOR0;  // Pixel color    
};

// MATERIAL SHADERS

MaterialVertexShaderOutput MaterialVertexShader( VSInput input )
{
    MaterialVertexShaderOutput output;
	float4 worldPosition	= mul( input.Position, WorldMatrix );
    float4 viewPosition		= mul( worldPosition, ViewMatrix );
    output.Position			= mul( viewPosition, ProjectionMatrix );   
    output.Position2		= output.Position; 
    output.Uv				= input.TexCoords;
    return output;    
}

MaterialPixelShaderOutput MaterialPixelShader( MaterialVertexShaderOutput input ) 
{ 
    MaterialPixelShaderOutput output;
    float2 uv = input.Uv;
    uv.y=1-uv.y;
	float4 tcol=tex2D (diffuseSample, uv);
	output.Color = tcol;
    return output;
}


// Techniques

technique Standard
{
    pass P0
    {   
        VertexShader = compile vs_3_0 MaterialVertexShader();
        PixelShader  = compile ps_3_0 MaterialPixelShader(); 
    }
}


