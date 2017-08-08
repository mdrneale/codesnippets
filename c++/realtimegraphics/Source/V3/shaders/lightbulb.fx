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

technique Standard
{
    pass P0
    {   
        VertexShader	= compile vs_3_0 LightVS();
        PixelShader		= compile ps_3_0 LightPS(); 
    }
}
