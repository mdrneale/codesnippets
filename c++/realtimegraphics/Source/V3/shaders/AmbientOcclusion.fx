// GLOBAL VARIABLES
float4x4 WorldMatrix		: WORLD;
float4x4 ProjectionMatrix	: PROJECTION;
float4x4 ViewMatrix			: VIEW;

texture PositionTexture		: positionMap;
sampler2D PositionSample	= sampler_state{	Texture = <PositionTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	AddressU = Mirror; AddressV = Mirror; };

texture NormalTexture		: normalMap;
sampler2D NormalSample		= sampler_state{	Texture = <NormalTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	AddressU = Mirror; AddressV = Mirror;	};




int		 NumberOfSamples;
float	 DistanceOfSample;


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
    float2 Pos				: TEXCOORD0;
};
struct PSOutput
{
  float4 Col 				: COLOR0;
};



//////////////////////////////////////////////////////////////////

VSOutput vVertexShader( VSInput input )
{
    VSOutput output		= (VSOutput)0;
    output.Position		= input.Position;
    output.Pos			= (input.Position.xy*0.5f)+0.5f;
    output.Pos.y		= 1.0f - output.Pos.y;
    return output;    
}

PSOutput pPixelShader( VSOutput input ) 
{ 
    PSOutput output			= (PSOutput)0;
	float3 normal			= mul(mul(tex2D(NormalSample	, input.Pos),ViewMatrix),ProjectionMatrix).xyz;
	float3 position			= mul(mul(tex2D(PositionSample	, input.Pos),ViewMatrix),ProjectionMatrix).xyz;
	float occlusion=0.0f;

	float2 movedpoint = clamp(float2(normal.x,normal.y)*(position.z),-1.0f,1.0f)*0.25f;
		float distance1 = position.z/60.0f;
		for (int a=0; a<16; a++)
		{
			for (int b=0; b<4; b++)
			{
				float2 samplepoint		=	float2(	input.Pos.x + movedpoint.x - sin((a/16.0f) * 2 * 3.14) * ((distance1)*(b/4.0f)),
													input.Pos.y + movedpoint.y - sin((a/16.0f) * 2 * 3.14) * ((distance1)*(b/4.0f)));
				float3 position2		=	tex2D(PositionSample, samplepoint).xyz;
				if (position2.x!=0.0f || position2.y!=0.0f || position2.z!=0.0f)
				{
					position2 = mul(mul(float4(position2,1.0f),ViewMatrix),ProjectionMatrix).xyz;
					float3 p=(position2-position);
					p=normalize(p);
					float dist = distance(position,position2);
					if (dist <= 2.00f)
					{
						occlusion				+= clamp(dot(normal.xyz,p)*(1.0f-(dist/2.00f)*(dist/2.00f)*(dist/2.00f)),0.0f,1.0f);
					}
					else
					{
						occlusion				+= 1.0f;
					}
				}
			}
		}
		
		output.Col = occlusion/64.0f;//(float4(movedpoint,0.0f,1.0f))*0.5+0.5;
		return output;
//	for (int a=0; a<16; a++)
//	{
//		for (int b=0; b<4; b++)
//		{
//			float2 samplepoint		=	float2(	input.Pos.x-sin((a/16.0f)*2*3.14)*((DistanceOfSample/4.0f)*b*b),
//												input.Pos.y-cos((a/16.0f)*2*3.14)*((DistanceOfSample/4.0f)*b*b));
//			float3 position2		=	tex2D(PositionSample, samplepoint).xyz;
//			if (position2.x!=0.0f || position2.y!=0.0f || position2.z!=0.0f)
//			{
//				float3 p=(position2-position);
//				p=normalize(p);
//				float dist = distance(position,position2);
//				if (dist <= 2.00f)
//				{
//					occlusion				+=  clamp(dot(normal.xyz,p)*(1.0f-(dist/2.00f)*(dist/2.00f)*(dist/2.00f)),0.0f,1.0f);
//					//div++;
//				}
//			}
//		}
//	}
//	occlusion/=div;
//	}
//	occlusion = pow((1.0f-occlusion),1);
//	output.Col = float4(occlusion,occlusion,occlusion,1.0f);
//	return output;
}

//PSOutput pPixelShader( VSOutput input ) 
//{ 
//    PSOutput output			= (PSOutput)0;
//	float3 normal			= tex2D(NormalSample, input.Pos).xyz;
//	float3 position			= tex2D(PositionSample, input.Pos).xyz;
//	float occlusion=1.0f;
//	float div=1.0f;
//	if (position.x!=0.0f || position.y!=0.0f || position.z!=0.0f)
//	{
//	for (int a=0; a<16; a++)
//	{
//		for (int b=0; b<4; b++)
//		{
//			float2 samplepoint		=	float2(	input.Pos.x-sin((a/16.0f)*2*3.14)*((DistanceOfSample/4.0f)*b*b),
//												input.Pos.y-cos((a/16.0f)*2*3.14)*((DistanceOfSample/4.0f)*b*b));
//			float3 position2		=	tex2D(PositionSample, samplepoint).xyz;
//			if (position2.x!=0.0f || position2.y!=0.0f || position2.z!=0.0f)
//			{
//				float3 p=(position2-position);
//				p=normalize(p);
//				float dist = distance(position,position2);
//				if (dist <= 2.00f)
//				{
//					occlusion				+=  clamp(dot(normal.xyz,p)*(1.0f-(dist/2.00f)*(dist/2.00f)*(dist/2.00f)),0.0f,1.0f);
//					div++;
//				}
//			}
//		}
//	}
//	occlusion/=div;
//	}
//	occlusion = pow((1.0f-occlusion),1);
//	output.Col = float4(occlusion,occlusion,occlusion,1.0f);
//	return output;
//}

// Techniques
technique Standard
{
    pass P0
    {   
        VertexShader = compile vs_3_0 vVertexShader();
        PixelShader  = compile ps_3_0 pPixelShader(); 
    }
}


