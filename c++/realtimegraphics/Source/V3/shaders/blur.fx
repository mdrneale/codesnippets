texture BlurTexture		: ImageMap;
sampler2D ImageSample		= sampler_state{	Texture = <BlurTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	AddressU = Clamp; AddressV = Clamp; };

// STRUCTS	////////////////////////////////////////////
struct VSInput	{	float4 Position	: POSITION;	};
struct VSOutput	{	float4 Position	: POSITION;	float2 Pos	: TEXCOORD0;	};
struct PSOutput	{	float4 Col		: COLOR0;	};
//////////////////////////////////////////////////////////////////

VSOutput BlurVertexShader( VSInput input )
{
    VSOutput output		= (VSOutput)0;
    output.Position		= input.Position;
    output.Pos			= (input.Position.xy*0.5f)+0.5f;
    output.Pos.y		= 1.0f - output.Pos.y;
    return output;    
}

PSOutput BlurPixelShaderV( VSOutput input ) 
{ 
	float BlurSize			=	30;
    PSOutput output			=	(PSOutput)0;
    float4 Final			=	tex2D(ImageSample, input.Pos);
	for (int a=0; a<BlurSize; a++)
	{
		float2 texco		=  float2(input.Pos.x-((BlurSize*0.5f)/200.0f)+((a/BlurSize)/200.0f) ,input.Pos.y);
		Final				+= tex2D(ImageSample, texco);
	}
	output.Col = Final/(BlurSize+1.0f);
    return output;
    
//    float4 Final			=	tex2D(ImageSample, input.Pos);
//    float  blur				=  1.0f-(Final.z/5.0f);
//	for (int a=0; a<15; a++)
//	{
//		float2 texco		=  float2(input.Pos.x-(0.01*blur*sin(2*3.14*(a/15.0f))),input.Pos.y+(0.01*blur*cos(2*3.14*(a/15.0f))));
//		Final				+= tex2D(ImageSample, texco);
//	}
//	output.Col = Final/16;
//    return output;
}

PSOutput BlurPixelShaderH( VSOutput input ) 
{ 
	float BlurSize			=	30;
    PSOutput output			=	(PSOutput)0;
    float4 Final			=	tex2D(ImageSample, input.Pos);
	for (int a=0; a<BlurSize; a++)
	{
		float2 texco		=  float2(input.Pos.x ,input.Pos.y-((BlurSize*0.5f)/200.0f)+((a/BlurSize)/200.0f));
		Final				+= tex2D(ImageSample, texco);
	}
	output.Col = Final/(BlurSize+1.0f);
    return output;
}

// Techniques
technique Standard
{
    pass P0
    {   
        VertexShader = compile vs_3_0 BlurVertexShader();
        PixelShader  = compile ps_3_0 BlurPixelShaderH();  
    }
    pass P1
    {   
        VertexShader = compile vs_3_0 BlurVertexShader();
        PixelShader  = compile ps_3_0 BlurPixelShaderV();  
    }
}


