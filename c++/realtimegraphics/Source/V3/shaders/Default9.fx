struct VSInput
{
    float4 Position : POSITION; 
    float3 Normal	: NORMAL;
    float3 Tangent	: TANGENT;
    float2 TexCoords : TEXCOORD0;
};

struct VSOutput
{
    float4 TransformedPosition : POSITION;
    float4 TransformedPosition2 : TEXCOORD0;
};

VSOutput VS1( VSInput input )
{
    VSOutput output;
    
    output.TransformedPosition = input.Position;
    output.TransformedPosition2 = input.Position;
    return output;    
}

VSOutput VS2( VSInput input )
{
    VSOutput output;
    
    output.TransformedPosition = input.Position;
    output.TransformedPosition.x-=1.0f;
    output.TransformedPosition2 = input.Position;
    return output;    
}

struct PSOutput
{
    float4 Color : COLOR0;  // Pixel color    
};

PSOutput PS1( VSOutput input ) 
{ 
    PSOutput output;
	
	output.Color = float4(input.TransformedPosition2.x,0,1,1);
    
    return output;
}

PSOutput PS2( VSOutput input ) 
{ 
    PSOutput output;
	
	output.Color = float4(0,1,1,0.5);
    
    return output;
}

technique Standard
{
    pass P0
    {   
        //CullMode = None;
        
        VertexShader = compile vs_2_0 VS1();
        PixelShader  = compile ps_2_0 PS1(); 
    }
    pass P1
    {   
        //CullMode = None;
        
        VertexShader = compile vs_2_0 VS2();
 //       PixelShader  = compile ps_2_0 PS2(); 
    }
}



