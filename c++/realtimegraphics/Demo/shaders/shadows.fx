// GLOBAL VARIABLES
float4x4 WorldMatrix		: WORLD;		//world matrix
float4x4 ProjectionMatrix	: PROJECTION;	//projection matrix of light
float4x4 ViewMatrix			: VIEW;			//view matrix of the light

texture PositionTexture		: positionMap;
sampler2D PositionSample	= sampler_state{	Texture = <PositionTexture>;};

texture ShadowTexture		: ShadowMap;
sampler2D ShadowSample		= sampler_state{	Texture = <ShadowTexture>;	};

// STRUCTS	////////////////////////////////////////////
struct SMAPVSInput
{
    float4 Position			: POSITION; 
};
struct SMAPVSOutput
{
    float4 Position			: POSITION;
    float4 PosInLightSpace	: TEXCOORD0;
};

struct SMAPPSOutput
{
  float4 rt 				: COLOR0;
};
//////////////////////////////////////////////////////////////////

SMAPVSOutput sVertexShader( SMAPVSInput input )
{
    SMAPVSOutput output		= (SMAPVSOutput)0;
    output.PosInLightSpace	= mul( input.Position, WorldMatrix );
    output.Position			= mul( mul(output.PosInLightSpace , ViewMatrix ), ProjectionMatrix );
    return output;    
}

SMAPPSOutput sPixelShader( SMAPVSOutput input ) 
{ 
    SMAPPSOutput output	= (SMAPPSOutput)0;
    input.PosInLightSpace	= mul( mul(input.PosInLightSpace , ViewMatrix ), ProjectionMatrix );
    output.rt				= input.PosInLightSpace.z/input.PosInLightSpace.w;
    return output;
}

// STRUCTS	////////////////////////////////////////////
struct INSCENEVSInput
{
    float4 Position			: POSITION; 
    float2 TexCoords		: TEXCOORD0;
};
struct INSCENEVSOutput
{
    float4 Position			: POSITION;
    float2 texcoords		: TEXCOORD1;
};
struct INSCENEPSOutput
{
	float4 rt 				: COLOR0;
};
//////////////////////////////////////////////////////////////////

INSCENEVSOutput s2VertexShader( INSCENEVSInput input )
{
    INSCENEVSOutput output		= (INSCENEVSOutput)0;
	///////////////////////////////////////////////////////////////
	// Position	///////////////////////////////////////////////////
    output.Position		= input.Position;
    output.texcoords	= (input.Position.xy*0.5f)+0.5f;
    output.texcoords.y	= 1.0f-output.texcoords.y;
    return output;    
}

INSCENEPSOutput s2PixelShader( INSCENEVSOutput input ) 
{ 
    INSCENEPSOutput output		=	(INSCENEPSOutput)0;
    float4	position			=	tex2D( PositionSample , input.texcoords );				// position from gbuffer
    float4	gbufPosInLightSpace	=	mul( mul( position , ViewMatrix ) , ProjectionMatrix );	// position in light space
	gbufPosInLightSpace			=	gbufPosInLightSpace/gbufPosInLightSpace.w;				// 3D point of position in light space
    float	realFragmentdepth	=	gbufPosInLightSpace.z;									// fragment Depth
    float2 uv					=	gbufPosInLightSpace.xy;									// uv's for shadow texture lookup
    uv+=float2(1.0f,1.0f);																	// move uvs from -1 to 1 To 0 to 2
    uv*=0.5f;																				// move uvs from  0 to 2 To 0 to 1
    uv.y=1.0f-uv.y;																			// flip Y
 
    if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)							// if uvs not in shadow map return black
    {
		output.rt = float4(0,0,0,1.0f);
	    return output;		
    } 

    float4	shadowposinlightspace	=	tex2D( ShadowSample , uv );							// position from shadow map
    float	lightfragmentdepth		=	shadowposinlightspace.x;

    if ((realFragmentdepth)/(lightfragmentdepth)>1.00098f)
    {
	 	output.rt = float4(0,0,0,1.0f);
	    return output;
	}
	output.rt = float4(1.0f,1.0f,1.0f,1.0f);
    return output;
}

// Techniques
technique Standard
{
    pass P0
    {   
        VertexShader = compile vs_3_0 sVertexShader();
        PixelShader  = compile ps_3_0 sPixelShader(); 
    }
    pass P1
    {   
        VertexShader = compile vs_3_0 s2VertexShader();
        PixelShader  = compile ps_3_0 s2PixelShader(); 
    }
}


