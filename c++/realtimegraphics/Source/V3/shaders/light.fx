// GLOBAL VARIABLES
float4x4 WorldMatrix		: WORLD;
float4x4 ProjectionMatrix	: PROJECTION;
float4x4 ViewMatrix			: VIEW;

float3	lightpos;
float3	lightcol;
float3	lightatt;

struct VS_INPUT 
{
	float4 position : POSITION;   // Vertex position in object space
	float3 normal	: NORMAL;      // Vertex normal in object space
	float3 tangent	: TANGENT;     // Vertex tangent in object space
	float2 tex		: TEXCOORD0;   // 
};

struct VS_OUTPUT 
{
	float4 position : POSITION;   // Pixel position in clip space	
	float4 position2: TEXCOORD1;   // Pixel position in clip space	
	float4 normal	: NORMAL;    // Pixel normal vector
};

struct PS_OUTPUT
{
    float4 Colour : COLOR0;  // Pixel color    
};

VS_OUTPUT LightVS( VS_INPUT input )
{
    VS_OUTPUT output;
    float4x4 WorldMatrix2=WorldMatrix;
 	WorldMatrix2[3][0]=0;
	WorldMatrix2[3][1]=0;
	WorldMatrix2[3][2]=0;	
	float4 normal			= float4(input.normal,1.0f);
	float4 newnormal        = mul( normal, WorldMatrix2 );  
	
	float4 worldPosition	= mul( input.position, WorldMatrix );
    float4 viewPosition		= mul( worldPosition, ViewMatrix );
    output.position			= mul( viewPosition, ProjectionMatrix );   
    output.position2		= worldPosition;
    output.normal			= newnormal/-newnormal.w;
    return output;    
}

PS_OUTPUT LightPS( VS_OUTPUT input ) 
{
	PS_OUTPUT output;
	float3 lightdir = normalize(lightpos-input.position2.xyz);
	float D=distance(lightpos,input.position2.xyz);
	float lightAttenuation = 1.0f/( lightatt.x + lightatt.y*D + lightatt.z*D * lightatt.z*D);
	float4	d = dot( lightdir,normalize(input.normal));
	output.Colour = float4(lightcol*d*lightAttenuation,1.0f);
	return output;
}

technique Standard
{
    pass simple
    {   
        VertexShader = compile vs_3_0 LightVS();
        PixelShader  = compile ps_3_0 LightPS(); 
    }
}