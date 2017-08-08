int		Time			: TIME;
float	Weight			: WEIGHT;
texture SceneTexture	: SceneMap;
texture TvStatic		: TvStaticMap;
sampler2D SceneSample		= sampler_state{	Texture	= <SceneTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};
sampler2D TvStaticSample	= sampler_state{	Texture = <TvStatic>;	};

struct VSInput
{
    float4 Position		: POSITION; 
};

struct TvStaticVSOutput
{
    float4 Position  : POSITION;
    float4 Position2 : TEXCOORD0;
};

struct TvStaticPSOutput
{
    float4 Color : COLOR0; 
};

TvStaticVSOutput TvStaticVS( VSInput input )
{
    TvStaticVSOutput output;
    output.Position			= input.Position;
    output.Position2		= output.Position; 
    return output;    
}

TvStaticPSOutput TvStaticPS( TvStaticVSOutput input ) 
{ 
    TvStaticPSOutput output;
    float2 uv	= float2(input.Position2.x,input.Position2.y);
    uv.y		= 0.0f - uv.y;
    uv			= (uv * 0.5f) + 0.5f;
	float4 scol	= tex2D(SceneSample, uv);
	float x		= (float)(Time % 4);
	float y		= (float)(Time / 4);
	uv			= uv * 20.0f;
	uv			= uv * 0.25f;
	uv.x		= uv.x + (0.25f * x);
	uv.y		= uv.y + (0.25f * y);
	float4 tvcol= tex2D(TvStaticSample, uv);
	tvcol.x=(scol.x - Weight) + (tvcol.x*Weight);
	tvcol.y=(scol.y - Weight) + (tvcol.y*Weight);
	tvcol.z=(scol.z - Weight) + (tvcol.z*Weight);
	output.Color = tvcol;    
    return output;
}

technique TvStaticRender
{
    pass P0
    {   
        VertexShader	= compile vs_3_0 TvStaticVS();
        PixelShader		= compile ps_3_0 TvStaticPS(); 
    }
}
