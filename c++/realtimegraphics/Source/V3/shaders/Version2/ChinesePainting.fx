// GLOBAL VARIABLES
float4x4 WorldMatrix		: WORLD;
float4x4 ProjectionMatrix	: PROJECTION;
float4x4 ViewMatrix			: VIEW;

texture		ColourTexture	: ColourMap;
texture		NormalTexture	: NormalMap;
texture		PositionTexture	: positionMap;

sampler2D	ColourSample	= sampler_state{	Texture = <ColourTexture>;		MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;};
sampler2D	NormalSample	= sampler_state{	Texture = <NormalTexture>;		MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;};
sampler2D	PositionSample	= sampler_state{	Texture = <PositionTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;};

float3	BaseCol;
float3	conedir;
float3	lightpos;
float3	lightatt;
float	fov;

// STRUCTS	////////////////////////////////////////////
struct GBufferVSInput
{
    float4 Position			: POSITION; 
    float3 Normal			: NORMAL;
    float3 Tangent			: TANGENT;
    float2 TexCoords		: TEXCOORD0;
};
struct GBufferVSOutput
{
    float4 Position			: POSITION;
    float4 WorldPosition	: TEXCOORD0;
    float3 Normal			: NORMAL;
    float3 Tangent			: TANGENT;
    float2 TexCoords		: TEXCOORD1;
};
struct GBufferPSOutput
{
  float4 rt[3] 				: COLOR0;
};
//////////////////////////////////////////////////////////////////

GBufferVSOutput GBufferVertexShader( GBufferVSInput input )
{
    GBufferVSOutput output			= (GBufferVSOutput)0;
    float4x4 WorldMatrix2			= WorldMatrix;
 	WorldMatrix2[3][0]				= 0;
	WorldMatrix2[3][1]				= 0;
	WorldMatrix2[3][2]				= 0;	
	float4 normal					= float4(input.Normal,1.0f);
	float4 n						= mul( normal, WorldMatrix2 ); 
	float4 t						= mul( float4(input.Tangent,1.0f), WorldMatrix2 ); 
    // Normal	///////////////////////////////////////////////////
	output.Normal					= (n/n.w).xyz;
	output.Tangent					= (t/t.w).xyz;
	///////////////////////////////////////////////////////////////

	// Position	///////////////////////////////////////////////////
    output.WorldPosition			= mul( input.Position, WorldMatrix );
    output.Position					= mul( mul( output.WorldPosition, ViewMatrix ), ProjectionMatrix );   
    ///////////////////////////////////////////////////////////////
    
	// Texture Coordinates /////////////////////////////////////////////////////////////
    input.TexCoords.y				= 1.0f-input.TexCoords.y;
	output.TexCoords				= input.TexCoords;
	///////////////////////////////////////////////////////////////
    return output;    
}

GBufferPSOutput GBufferPixelShader( GBufferVSOutput input ) 
{ 
    GBufferPSOutput output			= (GBufferPSOutput)0;
    output.rt[0]					= input.WorldPosition;
    
	float3 b						= cross(input.Normal, input.Tangent);
	float3 Normaladjust				= tex2D(NormalSample, input.TexCoords).xyz;
    Normaladjust					= (Normaladjust -0.5f) * 2.0f;
    float3x3 normalmatrix;
 	normalmatrix[2]					= input.Normal;
	normalmatrix[0]					= input.Tangent;
	normalmatrix[1]					= b;	

    output.rt[1]					= float4(normalize(mul(Normaladjust.xyz , normalmatrix)),0.0f);
	output.rt[2]					= float4(BaseCol,1.0f);
    return output;
}


// Techniques
technique Standard
{
    pass GBuffer
    {   
        VertexShader = compile vs_3_0 GBufferVertexShader	();
        PixelShader  = compile ps_3_0 GBufferPixelShader	(); 
    }
}

