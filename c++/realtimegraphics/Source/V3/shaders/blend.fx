texture Texture1	: Map1;
texture Texture2	: Map2;
sampler2D Sample1	= sampler_state{	Texture	= <Texture1>;	MinFilter = Linear;    MagFilter = Linear;    MipFilter = Linear;};
sampler2D Sample2	= sampler_state{	Texture = <Texture2>;	MinFilter = Linear;    MagFilter = Linear;    MipFilter = Linear;};

struct VSInput
{
    float4 Position		: POSITION; 
};

struct VSOutput
{
    float4 Position  : POSITION;
    float4 Position2 : TEXCOORD0;
};

struct PSOutput
{
    float4 Color : COLOR0; 
};

VSOutput VS( VSInput input )
{
    VSOutput output;
    output.Position			= input.Position;
    output.Position2		= output.Position; 
    return output;    
}

PSOutput addPS( VSOutput input ) 
{ 
    PSOutput output;
    float2 uv	= float2(input.Position2.x,input.Position2.y);
    uv.y		= 0.0f - uv.y;
    uv			= (uv * 0.5f) + 0.5f;
	float4 col1	= tex2D(Sample1, uv);
	float4 col2	= tex2D(Sample2, uv);
	output.Color = col1+col2;    
    return output;
}

PSOutput multiplyPS( VSOutput input ) 
{ 
    PSOutput output;
    float2 uv	= float2(input.Position2.x,input.Position2.y);
    uv.y		= 0.0f - uv.y;
    uv			= (uv * 0.5f) + 0.5f;
	float4 col1	= tex2D(Sample1, uv);
	float4 col2	= tex2D(Sample2, uv);
	output.Color = col1*col2;    
    return output;
}

PSOutput subtractPS( VSOutput input ) 
{ 
    PSOutput output;
    float2 uv	= float2(input.Position2.x,input.Position2.y);
    uv.y		= 0.0f - uv.y;
    uv			= (uv * 0.5f) + 0.5f;
	float4 col1	= tex2D(Sample1, uv);
	float4 col2	= tex2D(Sample2, uv);
	output.Color = col1-col2;    
    return output;
}

PSOutput dividePS( VSOutput input ) 
{ 
    PSOutput output;
    float2 uv	= float2(input.Position2.x,input.Position2.y);
    uv.y		= 0.0f - uv.y;
    uv			= (uv * 0.5f) + 0.5f;
	float4 col1	= tex2D(Sample1, uv);
	float4 col2	= tex2D(Sample2, uv);
	output.Color = col1/col2;    
    return output;
}

technique Standard
{
    pass add
    {   
        VertexShader	= compile vs_3_0 VS();
        PixelShader		= compile ps_3_0 addPS(); 
    }
    pass multiply
    {   
        VertexShader	= compile vs_3_0 VS();
        PixelShader		= compile ps_3_0 multiplyPS(); 
    }
    pass subtract
    {   
        VertexShader	= compile vs_3_0 VS();
        PixelShader		= compile ps_3_0 subtractPS(); 
    }
    pass divide
    {   
        VertexShader	= compile vs_3_0 VS();
        PixelShader		= compile ps_3_0 dividePS(); 
    }
}
