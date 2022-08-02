#include "StdH.h"
#include <Shaders/Common.h>

static const INDEX ctTextures = 1;
static const INDEX ctUVMaps   = 1;
static const INDEX ctColors   = 1;
static const INDEX ctFloats   = 0;
static const INDEX ctFlags    = 2;
static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms = 1;

static const INDEX iBaseTexture = 0;
static const INDEX iBaseUVMap   = 0;
static const INDEX iBaseColor   = 0;
static const INDEX iBaseFloat   = 0;
static const INDEX iBaseVP = 0;
static const INDEX iBasePP = 0;

SHADER_MAIN(Base)
{
	// Set texture wrapping
	shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
	// Set depth test
	shaEnableDepthTest();
	shaDepthFunc(GFX_LESS_EQUAL); 
	// Set alpha test
	shaDisableAlphaTest();
	
	shaSetTexture(iBaseTexture,0);
	shaSetUVMap(iBaseUVMap);
	shaSetColor(iBaseColor);
	
	const COLOR colModelColor = MulColors(shaGetModelColor(),shaGetCurrentColor());
	const BOOL bDoubleSided = shaGetFlags()&BASE_DOUBLE_SIDED;
	const BOOL bOpaque = (colModelColor&0xFF)==0xFF;
	const BOOL bHardwareShader = shaUseHWShaders();
	
	if(bHardwareShader) 
	{
		GFXColor colBase;
		colBase.MultiplyRGBA( (GFXColor)shaGetModelColor(), (GFXColor)shaGetCurrentColor());
		const ShaderRegister srBaseColor(colBase);
		
		shaSetFogTextureUnit(1, bOpaque);
		shaSetVertexProgram(iBaseVP);
		shaSetPixelProgram(iBasePP);
		shaSetPixelProgramConst( 0, &srBaseColor, 1);
		// prepare fog and haze
		shaPrepareFogAndHaze(bOpaque);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		shaSetDefaultConstantRegisters();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	}
	else 
	{
		// enable overbrightning
		shaEnableOverBrightning();
		// calculate shading
		shaCalculateLight();
		// prepare fog and haze
		shaPrepareFogAndHaze(bOpaque);
	}
	
	// if surface is opaque
	if(bOpaque) 
	{
		// disable blend
		shaDisableBlend();
		// enable depth write
		shaEnableDepthWrite();
		// if double sided flag is on
		if(bDoubleSided) 
		{
			// no face culling 
			shaCullFace(GFX_NONE);
		}
		else 
		{
			// back face culling
			shaCullFace(GFX_BACK);
		}
	}
	else 
	{ // else surface is translucent
		// Set blend mode
		shaEnableBlend();
		shaDisableDepthWrite();
		if(bHardwareShader) 
		{
			shaBlendFunc(GFX_ONE, GFX_SRC_ALPHA);
		}
		else 
		{
			// Adjust alpha for fog
			shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
			shaAttenuateAlphaForFogAndHaze();
		}
		
		// if double sided
		if(bDoubleSided) 
		{
			// render back side
			shaCullFace(GFX_FRONT);
			shaRender();
		}
		// back face culling
		shaCullFace(GFX_BACK);
	}
	// render front side
	shaRender();
	
	if(!bHardwareShader) 
	{
		// disable overbrighning
		shaDisableOverBrightning();
		// if surface is opaque
		if(bOpaque) 
		{
			// render fog and haze pass for surface
			shaDoFogAndHazePass();
		}
	}
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(Base,ShaderDesc *&pshDesc)
{
	static bool bInit = false;
	static ShaderDesc shDescMe;
	if(!bInit)
	{
		bInit = true;
		shDescMe.sd_astrTextureNames.New(ctTextures);
		shDescMe.sd_astrTexCoordNames.New(ctUVMaps);
		shDescMe.sd_astrColorNames.New(ctColors);
		shDescMe.sd_astrFloatNames.New(ctFloats);
		shDescMe.sd_astrFlagNames.New(ctFlags);
		
		shDescMe.sd_astrTextureNames[0] = "Base texture";
		shDescMe.sd_astrTexCoordNames[0] = "Base uvmap";
		shDescMe.sd_astrColorNames[0] = "Base color";
		shDescMe.sd_astrFlagNames[0] = "Double sided";
		shDescMe.sd_astrFlagNames[1] = "Full bright";
		shDescMe.sd_strShaderInfo = "Basic shader";
		
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_NORMAL_STREAM;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(Base, CTString &strVPCode, INDEX iVertexProgram)
{
	ASSERT(iVertexProgram==iBaseVP);
#if 0
	strVPCode = 
		// Global declarations;
		" FUNCTION 'DotProduct' = 'dp3';                                \n"
		" FUNCTION 'DotProduct4' = 'dp4';                               \n"
		" FUNCTION 'Mad' = 'mad';                                       \n"
		" FUNCTION 'Max' = 'max';                                       \n"
		" FUNCTION 'Min' = 'min';                                       \n"
		" FUNCTION 'Add' = 'add';                                       \n"
		" FUNCTION 'Mov' = 'mov';                                       \n"
		" FUNCTION 'Mul' = 'mul';                                       \n"
		
		" REGISTER 'rConst' = 'c7';                                     \n"
		" FLOAT3D  'vVertex' = 'r0';                                    \n"
		" FLOAT3D  'vNormal' = 'r1';                                    \n"
		" FLOAT3D  'vLightDir' = 'c4';                                  \n"
		" FLOAT3D  'vLightColor' = 'c5';                                \n"
		" FLOAT3D  'vLightAmbient' = 'c6';                              \n"
		" FLOAT    '0.0f' = 'c7.x';                                     \n"
		" FLOAT    '1.0f' = 'c7.y';                                     \n"
		" FLOAT    '0.5f' = 'c7.w';                                     \n"
		" FLOAT    '2.0f' = 'c7.z';                                     \n"
		
		// Local declarations
		" REGISTER 'rDot' = 'r4';                                       \n"
		" REGISTER 'vVertexColor' = 'r5.xyz';                           \n"
		" REGISTER 'rVertexColor' = 'r5';                               \n"
		" REGISTER 'rOutputColor' = 'oD0';                              \n"
		
		" // Normal dot light                                           \n"
		" rDot.w = DotProduct(vNormal, vLightDir);                      \n"
		" // Clamp result of dotproduct [0.0f - 1.0f]                   \n"
		" rDot.w = Min(rDot.w, 1.0f);                                   \n"
		" rDot.w = Max(rDot.w, 0.0f);                                   \n"
		" // Calculate light color                                      \n"
		" vVertexColor = Mul(vLightColor, rDot.www);                    \n"
		" // Add ambient color                                          \n"
		" vVertexColor = Add(vVertexColor, vLightAmbient);              \n"
		" // Output as vertex color                                     \n"
		" rVertexColor.w = Mov(1.0f);                                   \n"
		" rOuputColor = Min(rVertexColor, rConst.yyyy);                 \n"
		;
#endif
	
	strVPCode = CALCULATE_SHADING_VP
		STANDARD_OUTPUT_VP
		;
	
}

// Fc = (Bc*Ba)
// Fa = (1-Ba)
SHADER_PCODE(Base, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	ASSERT(iPixelProgram==iBasePP);
	if(eFogType==FT_NONE) 
	{
		strPPCode = "tex    t0                     \n" // load base texture
					"mul    t0,     t0,    c0      \n" // mul base texture with base color
					"mul    r0.rgb, t0,    t0.a    \n" // Fc=Bc*Ba
					"mul_x2 r0.rgb, r0,    v0      \n" // Shade pixel
					"+mov   r0.a,   1-t0.a         \n" // Fa = (1-Ba)
					;                             
	}
	else if(eFogType==FT_OPAQUE) 
	{
		strPPCode = "tex    t0                     \n" // load base texture
					"tex    t1                     \n" // load fog texture
					"mul    t0,     t0,    c0      \n" // mul base texture with base color
					"mul    t1,     t1,    c7      \n" // mul fog texture with fog color
					"mul    r0.rgb, t0,    t0.a    \n" // Fc = Bc*Ba
					"mul_x2 r0.rgb, r0,    v0      \n" // Shade pixel
					"+mov   r0.a,   1-t0.a         \n" // Fa = (1-Ba)
					"lrp    r0.rgb, t1.a,  t1,  r0 \n" // Add fog
					;
	}
	else if(eFogType==FT_NON_OPAQUE) 
	{
		strPPCode = "tex    t0                     \n" // load base texture
					"tex    t1                     \n" // load fog texture
					"mul    t0,     t0,    c0      \n" // mul base texture with base color
					"mul    t1,     t1,    c7      \n" // mul fog texture with fog color
					"mul    t0.a,   t0,    1-t1.a  \n" // attenuate base tex alpha with fog alpha
					"mul    r0.rgb, t0,    t0.a    \n" // Fc = Bc*Ba
					"mul_x2 r0.rgb, r0,    v0      \n" // Shade pixel
					"+mov   r0.a,   1-t0.a         \n" // Fa = (1-Ba)
					;
	}
}
