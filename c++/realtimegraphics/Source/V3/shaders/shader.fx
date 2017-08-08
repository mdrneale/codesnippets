float4x4 wvp : WorldViewProjection;

struct VS_IN
{
	float4 Position : POSITION;
};

struct VS_OUT
{
	float4 test : TEXCOORD0;
};

VS_OUT VS_Color(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	
	output.test = mul(input.Position,wvp);
	output.test.w=1.0;
	return output;
}

struct PS_OUT
{
	float4 Color : COLOR;
};

PS_OUT PS_Color(VS_OUT input)
{
	PS_OUT output = (PS_OUT)0;
	
	output.Color =  input.test;
	
	return output;
}

technique PlanetShader
{
	pass Colors
	{
		AlphaBlendEnable = False;
		CullMode = CCW;
		VertexShader = compile vs_2_0 VS_Color();
		PixelShader = compile ps_2_0 PS_Color();		
	}
}