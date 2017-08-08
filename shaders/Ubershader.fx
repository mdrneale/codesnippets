// GLOBAL VARIABLES
float4x4 WorldMatrix		: WORLD;
float4x4 ProjectionMatrix	: PROJECTION;
float4x4 ViewMatrix			: VIEW;

texture		ColourTexture	: ColourMap;//;	
sampler2D	ColourSample	= sampler_state{	Texture = <ColourTexture>;		MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;};

texture		NormalTexture	: NormalMap;
sampler2D	NormalSample	= sampler_state{	Texture = <NormalTexture>;		MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

sampler2D	bloomSample		= sampler_state{	Texture = <NormalTexture>;		MinFilter = Linear;	MagFilter = Linear;	};

texture		PositionTexture	: positionMap;//MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;
sampler2D	PositionSample	= sampler_state{	Texture = <PositionTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

texture		ShadowTexture	: ShadowMap;
sampler2D	ShadowSample	= sampler_state{	Texture = <ShadowTexture>;	MinFilter = Linear;	MagFilter = Linear;	MipFilter = Linear;	};

sampler2D	shadSample		= sampler_state{	Texture = <ColourTexture>;		};
sampler2D	shad2Sample	= sampler_state{	Texture = <PositionTexture>;	};
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
    float4x4 WorldMatrix2	= WorldMatrix;
 	WorldMatrix2[3][0]=0;
	WorldMatrix2[3][1]=0;
	WorldMatrix2[3][2]=0;	
	float4 normal			= float4(input.Normal,1.0f);
	float4 n    = mul( normal, WorldMatrix2 ); 
	float4 t	= mul( float4(input.Tangent,1.0f), WorldMatrix2 ); 

    // Normal	///////////////////////////////////////////////////
	output.Normal			= (n/n.w).xyz;
	output.Tangent			= (t/t.w).xyz;
	///////////////////////////////////////////////////////////////
	// Position	///////////////////////////////////////////////////
    output.WorldPosition	= mul( input.Position, WorldMatrix );
    output.Position			= mul( mul( output.WorldPosition, ViewMatrix ), ProjectionMatrix );   
	// Texture Coordinates /////////////////////////////////////////////////////////////
    input.TexCoords.y		= 1.0f-input.TexCoords.y;
	output.TexCoords		= input.TexCoords;
	///////////////////////////////////////////////////////////////
    return output;    
}

GBufferPSOutput GBufferPixelShader( GBufferVSOutput input ) 
{ 
    GBufferPSOutput output		= (GBufferPSOutput)0;
    output.rt[0]		= input.WorldPosition;
	float3 b			= cross(input.Normal, input.Tangent);
	float3 Normaladjust = tex2D(NormalSample, input.TexCoords).xyz;
    Normaladjust = (Normaladjust -0.5f) * 2.0f;

    float3x3 normalmatrix;
 	normalmatrix[2]	= input.Normal;
	normalmatrix[0]	= input.Tangent;
	normalmatrix[1]	= b;	
	float3 nn = mul(Normaladjust.xyz , normalmatrix);
	input.Normal		= normalize(nn);//(input.Normal);// + Normaladjust.xyz)/2;

    output.rt[1]		= float4(input.Normal,0.0f);
	output.rt[2]		= float4(BaseCol,1.0f);
    return output;
}

// STRUCTS	////////////////////////////////////////////
struct AmbientVSInput {	float4 Position	: POSITION;	};
struct AmbientVSOutput{    float4 Position	: POSITION;	float2 TexCoords	: TEXCOORD0; };
struct AmbientPSOutput{	float4 rt		: COLOR0;	};
//////////////////////////////////////////////////////////////////

float3 pSphere[16] = 
{
	float3(0.53812504, 0.18565957, -0.43192),		float3(0.13790712, 0.24864247, 0.44301823),
	float3(0.33715037, 0.56794053, -0.005789503),	float3(-0.6999805, -0.04511441, -0.0019965635),
	float3(0.06896307, -0.15983082, -0.85477847),	float3(0.056099437, 0.006954967, -0.1843352),
	float3(-0.014653638, 0.14027752, 0.0762037),	float3(0.010019933, -0.1924225, -0.034443386),
	float3(-0.35775623, -0.5301969, -0.43581226),	float3(-0.3169221, 0.106360726, 0.015860917),
	float3(0.010350345, -0.58698344, 0.0046293875),	float3(-0.08972908, -0.49408212, 0.3287904),
	float3(0.7119986, -0.0154690035, -0.09183723),	float3(-0.053382345, 0.059675813, -0.5411899),
	float3(0.035267662, -0.063188605, 0.54602677),	float3(-0.47761092, 0.2847911, -0.0271716)
};

float findocclusion(float3 pos,float3 normal,float2 uv, float radius)
{
	float falloff= 0.025f;
	float ao=16.0f;
	for (int a=0; a<16; a++)
	{
		float3 SecondPos = pSphere[a];//sign(dot(pSphere[a],normal))*pSphere[a];
		SecondPos*=0.2f*clamp( ((1-(radius))*(1-(radius))),0,1);
		//SecondPos.xy*=(1.0f-abs(SecondPos.z));
		SecondPos.xy+=uv;
		float4 temp				= tex2D(PositionSample	, SecondPos.xy);
		float3 SecondPosition	= temp.xyz/temp.w;
		
		float dist = distance(pos,SecondPosition)/falloff;
		float3 normaltopoint = normalize(SecondPosition-pos);
		float dif = dot(normal,normaltopoint);
		if (dif>0.15f)
		{
			ao-=(0.15f+(1.0f*0.85f));//clamp(dot(normal,-normalize(SecondPosition-pos)*(dist/falloff)),0.0f,1.0f);
		}
	}
	return ao/16;
}
AmbientVSOutput AmbientVertexShader( AmbientVSInput input )
{
    AmbientVSOutput output		= (AmbientVSOutput)0;
    output.Position		= input.Position;
    output.TexCoords	= (input.Position.xy*0.5f)+0.5f;
    output.TexCoords.y	= 1.0f-output.TexCoords.y;
    return output;    
}

AmbientPSOutput AmbientPixelShader( AmbientVSOutput input ) 
{
    AmbientPSOutput output		= (AmbientPSOutput)0;

	float3 screenNormal		= tex2D(NormalSample	, input.TexCoords).xyz;//,ViewMatrix),ProjectionMatrix).xyz;
	float4 temp				= tex2D(PositionSample	, input.TexCoords);//,ViewMatrix),ProjectionMatrix);
	float3 screenPosition	= temp.xyz;///temp.w;
	temp					= mul(mul(temp,ViewMatrix),ProjectionMatrix);
	float  ratio			= (temp.w/400.0f);
	
    float3  colour		= tex2D(ColourSample,	input.TexCoords).xyz;
    float	occlusion	= findocclusion(screenPosition,screenNormal,input.TexCoords, ratio);
    
	output.rt			= float4(colour*BaseCol*occlusion,1.0f);
	//output.rt.r=	ratio;
	
	return output;
}

// STRUCTS	////////////////////////////////////////////
struct ShadowMapVSInput{    float4 Position			: POSITION; };
struct ShadowMapVSOutput{	float4 Position			: POSITION; float4 PosInLightSpace	: TEXCOORD0; };
struct ShadowMapPSOutput{   float4 rt 				: COLOR0; };
//////////////////////////////////////////////////////////////////

ShadowMapVSOutput ShadowMapVertexShader( ShadowMapVSInput input )
{
    ShadowMapVSOutput output	= (ShadowMapVSOutput)0;
    output.PosInLightSpace		= mul( input.Position, WorldMatrix );
    output.Position				= mul( mul(output.PosInLightSpace , ViewMatrix ), ProjectionMatrix );
    return output;    
}

ShadowMapPSOutput ShadowMapPixelShader( ShadowMapVSOutput input ) 
{ 
    ShadowMapPSOutput output	= (ShadowMapPSOutput)0;
    input.PosInLightSpace		= mul( mul(input.PosInLightSpace , ViewMatrix ), ProjectionMatrix );
    output.rt					= input.PosInLightSpace.z/input.PosInLightSpace.w;
    return output;
}

// STRUCTS	////////////////////////////////////////////
struct ShadowMapToSceneVSInput{		float4 Position	: POSITION;	float2 TexCoords	: TEXCOORD0; };
struct ShadowMapToSceneVSOutput{	float4 Position	: POSITION;	float2 texcoords	: TEXCOORD1; };
struct ShadowMapToScenePSOutput{	float4 rt 		: COLOR0; };
//////////////////////////////////////////////////////////////////

ShadowMapToSceneVSOutput ShadowMapToSceneVertexShader( ShadowMapToSceneVSInput input )
{
    ShadowMapToSceneVSOutput output		= (ShadowMapToSceneVSOutput)0;
	///////////////////////////////////////////////////////////////
	// Position	///////////////////////////////////////////////////
    output.Position		= input.Position;
    output.texcoords	= (input.Position.xy*0.5f)+0.5f;
    output.texcoords.y	= 1.0f-output.texcoords.y;
    return output;    
}

ShadowMapToScenePSOutput ShadowMapToScenePixelShader( ShadowMapToSceneVSOutput input ) 
{ 
    ShadowMapToScenePSOutput output		=	(ShadowMapToScenePSOutput)0;
    float4	position			=	tex2D( shad2Sample , input.texcoords );				// position from gbuffer
    float4	gbufPosInLightSpace	=	mul( mul( position , ViewMatrix ) , ProjectionMatrix );	// position in light space
	gbufPosInLightSpace			=	gbufPosInLightSpace/gbufPosInLightSpace.w;				// 3D point of position in light space
    float	realFragmentdepth	=	gbufPosInLightSpace.z;									// fragment Depth
    float2 uv					=	gbufPosInLightSpace.xy;									// uv's for shadow texture lookup
    uv+=float2(1.0f,1.0f);																	// move uvs from -1 to 1 To 0 to 2
    uv*=0.5f;																				// move uvs from  0 to 2 To 0 to 1
    uv.y=1.0f-uv.y;																			// flip Y
 
    if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)							// if uvs not in shadow map return black
    {
		output.rt = 0;
	    return output;		
    } 

    float4	shadowposinlightspace	=	tex2D( shadSample , uv );							// position from shadow map
    float	lightfragmentdepth		=	shadowposinlightspace.x;

    if ((realFragmentdepth)-(lightfragmentdepth)>0.001f)
    {
	 	output.rt = 0;
	    return output;
	}
	output.rt = 1.0f;
    return output;
}

// STRUCTS	////////////////////////////////////////////
struct BlurVSInput{		float4 Position	: POSITION;	};
struct BlurVSOutput{	float4 Position	: POSITION;	float2 texcoords	: TEXCOORD1; };
struct BlurPSOutput{	float4 rt 		: COLOR0; };
//////////////////////////////////////////////////////////////////

BlurVSOutput BlurVertexShader( BlurVSInput input )
{
    BlurVSOutput output		= (BlurVSOutput)0;
	///////////////////////////////////////////////////////////////
	// Position	///////////////////////////////////////////////////
    output.Position		= input.Position;
    output.texcoords	= (input.Position.xy*0.5f)+0.5f;
    output.texcoords.y	= 1.0f-output.texcoords.y;
    return output;    
}

BlurPSOutput BlurSceneHPixelShader( BlurVSOutput input ) 
{
	BlurPSOutput output		= (BlurPSOutput)0;
    float	colour	= 0;		

    float	Samples = 4.0f;	
    for (int a = 0; a < Samples; a++)
	{
		float offset = ( -0.5f + (a/(Samples-1.0f)) ) / 128.0f;
		colour += tex2D( NormalSample , float2(input.texcoords.x + offset,input.texcoords.y) ).x;
	}	
	colour /= Samples;
	output.rt = float4(colour,colour,colour,colour);
    return output;    
}

BlurPSOutput BlurSceneVPixelShader( BlurVSOutput input ) 
{
	BlurPSOutput output		= (BlurPSOutput)0;
    float	colour	= 0;		

    float	Samples = 4.0f;	
    for (int a = 0; a < Samples; a++)
	{
		float offset = ( -0.5f + (a/(Samples-1.0f)) ) / 128.0f;
		colour += tex2D( NormalSample , float2(input.texcoords.x ,input.texcoords.y + offset) ).x;
	}	
	colour /= Samples;
	output.rt = float4(colour,colour,colour,colour);
    return output;    
}

// STRUCTS	////////////////////////////////////////////
struct ShadeTheSceneVSInput {	float4 Position	: POSITION;	};
struct ShadeTheSceneVSOutput{   float4 Position	: POSITION;	float2 TexCoords	: TEXCOORD0; };
struct ShadeTheScenePSOutput{	float4 rt[2]	: COLOR0;	};
//////////////////////////////////////////////////////////////////

ShadeTheSceneVSOutput ShadeTheSceneVertexShader( ShadeTheSceneVSInput input )
{
    ShadeTheSceneVSOutput output		= (ShadeTheSceneVSOutput)0;
    output.Position		= input.Position;
    output.TexCoords	= (input.Position.xy*0.5f)+0.5f;
    output.TexCoords.y	= 1.0f-output.TexCoords.y;
    return output;    
}

ShadeTheScenePSOutput ShadeTheScenePixelShader( ShadeTheSceneVSOutput input ) 
{
    ShadeTheScenePSOutput output		= (ShadeTheScenePSOutput)0;
	float shadow			= tex2D(ShadowSample, input.TexCoords).x;
 	output.rt[0]			= float4(0,0,0,1);
    if (shadow!=0)
    {
	    float3 normal		= tex2D(NormalSample,	input.TexCoords).xyz;
		float3 position		= tex2D(PositionSample, input.TexCoords).xyz;
		float3 Colour		= tex2D(ColourSample, input.TexCoords).xyz;
		float  D			= distance(lightpos,position);
		float3 lightdir		= normalize(lightpos-position);
		float  lightAttenuation	= 1.0f / ( lightatt.x + lightatt.y*D );//+ lightatt.z*D * D);
		float  d			= clamp(dot( lightdir , normalize( normal ) ) , 0 , 1 );
		float  coneAttenuation	= -dot( conedir, lightdir );
		float t=0.0;
		if ((coneAttenuation)>cos(fov*0.5f))
		{
			t=pow(((coneAttenuation-(cos(fov*0.5f)))/(1-cos(fov*0.5f))),2);
		}
		float3 diff		=	saturate(dot(normal, lightdir));
		float3 Reflect	=	normalize(2 * diff * normal - lightdir);
		float4 specular =	pow(saturate(dot(Reflect, float3(0,0,1.0f))), 28);
		output.rt[0]		= float4(Colour*shadow*d*t*lightAttenuation*BaseCol,1.0f);
	}
	output.rt[1] = output.rt[0];
	return output;
}

//////////////////////////////////////////////////////////////////////
struct DrawLightBulbsVSInput	{	float4 Position		: POSITION;	float2 TexCoords	: TEXCOORD0; };
struct DrawLightBulbsVSOutput	{   float4 Position		: POSITION;	float2 Uvs			: TEXCOORD0; };
struct DrawLightBulbsPSOutput	{   float4 Color : COLOR0; };

DrawLightBulbsVSOutput DrawLightBulbsVertexShader( DrawLightBulbsVSInput input )
{
    DrawLightBulbsVSOutput	output;
    output.Uvs				= input.TexCoords; 
   	float4 worldPosition	= mul( input.Position, WorldMatrix );
    float4 viewPosition		= mul( worldPosition, ViewMatrix );
    output.Position			= mul( viewPosition, ProjectionMatrix ); 
    return output;    
}

DrawLightBulbsPSOutput DrawLightBulbsPixelShader( DrawLightBulbsVSOutput input ) 
{ 
    DrawLightBulbsPSOutput output;
	float4 scol	= tex2D(NormalSample, input.Uvs);
	output.Color = scol*float4(BaseCol,1.0f);
    return output;
}

// STRUCTS	////////////////////////////////////////////
struct ToneMapVSInput{	float4 Position	: POSITION; };
struct ToneMapVSOutput{	float4 Position	: POSITION;	float2 uv	: TEXCOORD0;	};
struct ToneMapPSOutput{	float4 rt[2]	: COLOR0;	};
//////////////////////////////////////////////////////////////////

ToneMapVSOutput ToneMapVertexShader( ToneMapVSInput input )
{
    ToneMapVSOutput output	= (ToneMapVSOutput)0;
    output.Position			= input.Position;
    output.uv		=	(input.Position.xy*0.5f)+0.5f;
    output.uv.y		=	1.0f-output.uv.y;
    return output;    
}

ToneMapPSOutput ToneMapPixelShader( ToneMapVSOutput input ) 
{ 
    ToneMapPSOutput output	=	(ToneMapPSOutput)0;
    float4 Colour	=	tex2D(ColourSample	, input.uv);
    float b=fov;//Luminance;//,MinLuminance,MaxLuminance);
//    float max = MaxLuminance/b;
//    float min = MinLuminance/b;
    ///////////////////////////////////////////////////////
    output.rt[0]	=	Colour/float4(b,b,b,b);
    if (Colour.x>b || Colour.y>b || Colour.z>b )
    {
		output.rt[1]	=	Colour;
	}
	else
	{
		output.rt[1]	=	float4(0,0,0,0);	
	}
    return output;
}

// STRUCTS	////////////////////////////////////////////
struct BloomVSInput{		float4 Position	: POSITION;	};
struct BloomVSOutput{	float4 Position	: POSITION;	float2 texcoords	: TEXCOORD1; };
struct BloomPSOutput{	float4 rt 		: COLOR0; };
//////////////////////////////////////////////////////////////////

BloomVSOutput BloomVertexShader( BloomVSInput input )
{
    BloomVSOutput output		= (BloomVSOutput)0;
	///////////////////////////////////////////////////////////////
	// Position	///////////////////////////////////////////////////
    output.Position		= input.Position;
    output.texcoords	= (input.Position.xy*0.5f)+0.5f;
    output.texcoords.y	= 1.0f-output.texcoords.y;
    return output;    
}

BloomPSOutput BloomHPixelShader( BloomVSOutput input ) 
{
	BloomPSOutput output		= (BloomPSOutput)0;
    float4	colour	= float4(0,0,0,0);		

    float	Samples = 15.0f;	
    for (int a = 0; a < Samples; a++)
	{
		float offset = ( -0.5f + (a/(Samples-1.0f)) ) / 1280.0f;
		offset*=Samples;
		colour += tex2D( bloomSample , float2(input.texcoords.x + offset,input.texcoords.y) );
	}	
	colour /= Samples;
	output.rt = colour;
    return output;    
}

BloomPSOutput BloomVPixelShader( BloomVSOutput input ) 
{
	BloomPSOutput output		= (BloomPSOutput)0;
    float4	colour	= float4(0,0,0,0);		

    float	Samples = 15.0f;	
    for (int a = 0; a < Samples; a++)
	{
		float offset = ( -0.5f + (a/(Samples-1.0f)) ) / 800.0f;
		offset*=Samples;
		colour += tex2D( bloomSample , float2(input.texcoords.x,input.texcoords.y + offset) );
	}	
	colour /= Samples;
	colour += tex2D( PositionSample , input.texcoords );
	output.rt = colour;
    return output;    
}

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

// Techniques
technique Standard
{
    pass GBuffer
    {   
        VertexShader = compile vs_3_0 GBufferVertexShader	();
        PixelShader  = compile ps_3_0 GBufferPixelShader	(); 
    }
    pass Ambient
    {   
        VertexShader = compile vs_3_0 AmbientVertexShader();
        PixelShader  = compile ps_3_0 AmbientPixelShader(); 
    }
    pass ShadowMap
    {   
        VertexShader = compile vs_3_0 ShadowMapVertexShader();
        PixelShader  = compile ps_3_0 ShadowMapPixelShader(); 
    }
    pass ShadowMapToScene
    {   
        VertexShader = compile vs_3_0 ShadowMapToSceneVertexShader();
        PixelShader  = compile ps_3_0 ShadowMapToScenePixelShader(); 
    }
    pass BlurSceneH
    {   
        VertexShader = compile vs_3_0 BlurVertexShader();
        PixelShader  = compile ps_3_0 BlurSceneHPixelShader(); 
    }
    pass BlurSceneV
    {   
        VertexShader = compile vs_3_0 BlurVertexShader();
        PixelShader  = compile ps_3_0 BlurSceneVPixelShader(); 
    }
    pass ShadeTheScene
    {   
        VertexShader = compile vs_3_0 ShadeTheSceneVertexShader();
        PixelShader  = compile ps_3_0 ShadeTheScenePixelShader(); 
    }
    pass DrawLightBulbs
    {   
        VertexShader = compile vs_3_0 DrawLightBulbsVertexShader();
        PixelShader  = compile ps_3_0 DrawLightBulbsPixelShader(); 
    }
    pass ToneMap
    {   
        VertexShader = compile vs_3_0 ToneMapVertexShader();
        PixelShader  = compile ps_3_0 ToneMapPixelShader(); 
    }
    pass BloomH
    {   
        VertexShader = compile vs_3_0 BloomVertexShader();
        PixelShader  = compile ps_3_0 BloomHPixelShader(); 
    }
    pass BloomV
    {   
        VertexShader = compile vs_3_0 BloomVertexShader();
        PixelShader  = compile ps_3_0 BloomVPixelShader(); 
    }
    pass P0
    {   
        VertexShader	= compile vs_3_0 TvStaticVS();
        PixelShader		= compile ps_3_0 TvStaticPS(); 
    }
}

