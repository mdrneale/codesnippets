// GLOBAL VARIABLES
float4x4 WorldMatrix		: WORLD;

texture DisplayTexture		: TextureMap;
sampler2D TextureSample		= sampler_state{	Texture = <DisplayTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

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
    float2 TexCoords		: TEXCOORD0;
};
struct PSOutput
{
  float4 rt 				: COLOR0;
};
//////////////////////////////////////////////////////////////////

VSOutput DebugVertexShader( VSInput input )
{
    VSOutput output		= (VSOutput)0;
    output.Position	= mul( input.Position, WorldMatrix );
    output.TexCoords =  input.TexCoords;
    //output.TexCoords.y=1-output.TexCoords.y;
    return output;    
}

PSOutput DebugPixelShader( VSOutput input ) 
{ 
    PSOutput output	= (PSOutput)0;
    output.rt		= tex2D( TextureSample , input.TexCoords );
    return output;
}

// Techniques
technique Standard
{
    pass P0
    {   
        VertexShader = compile vs_3_0 DebugVertexShader();
        PixelShader  = compile ps_3_0 DebugPixelShader(); 
    }
}


