// GLOBAL VARIABLES
float4x4 WorldMatrix		: WORLD;
float4x4 ProjectionMatrix	: PROJECTION;
float4x4 ViewMatrix			: VIEW;

texture SceneTexture		: SceneMap;
sampler2D SceneSample		= sampler_state{	Texture = <SceneTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

float   Luminance;
float   MinLuminance;
float   MaxLuminance;

// STRUCTS	////////////////////////////////////////////
struct VSInput
{
    float4 Position			: POSITION; 
};
struct VSOutput
{
    float4 Position			: POSITION;
    float4 Pos				: COLOR1;
};
struct PSOutput
{
  float4 rt 				: COLOR0;
};
//////////////////////////////////////////////////////////////////

VSOutput ToneMapVertexShader( VSInput input )
{
    VSOutput output		= (VSOutput)0;
    output.Position		= output.Pos = input.Position;   
    return output;    
}

PSOutput ToneMapPixelShader( VSOutput input ) 
{ 
    PSOutput output	=	(PSOutput)0;
    float2 uv		=	(input.Pos.xy*0.5f)+0.5f;
    uv.y			=	1.0f-uv.y;
    float4 Colour	=	tex2D(SceneSample	, uv);
    float b=Luminance;//Luminance;//,MinLuminance,MaxLuminance);
    float max = MaxLuminance/b;
    float min = MinLuminance/b;
    Colour/=float4(b,b,b,b);
//    Colour=clamp(Colour,min,max);
//    Colour-=min;

	float nos=10.0f;
	float4 bloomColour=float4(0,0,0,0);	
	for (int a=0; a<nos; a++)
	{
		float2 texco		= float2((uv.x - 0.02f) + ((a/nos) * 0.04f) ,uv.y);
		bloomColour		   += tex2D(SceneSample	, texco);
		texco				= float2(uv.x ,(uv.y - 0.02f) + ((a/nos) * 0.04f) );
		bloomColour		   += tex2D(SceneSample	, texco);
	}
	bloomColour/=nos*2.0f;
    bloomColour/=float4(b,b,b,b);
	Colour+=(bloomColour);
	
    
    ///////////////////////////////////////////////////////
    output.rt		=	Colour;
    return output;
}

// Techniques
technique Standard
{
    pass P0
    {   
        VertexShader = compile vs_3_0 ToneMapVertexShader();
        PixelShader  = compile ps_3_0 ToneMapPixelShader(); 
    }
}


