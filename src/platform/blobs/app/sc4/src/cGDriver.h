#pragma once

// From SCGL. Code modified specifically for use in scion as hooks, and for personal tweaks.
// Also modified to use SDL instead of glfw.


#include <cstdio>
#include <string>
#include <string.h>
#include <vector>
#include "cIGZBuffer.h"
#include "cRZRefCount.h"
#include "cIGZGDriver.h"
#include "sGDMode.h"
#include "cIGZGBufferRegionExtension.h"
#include "cIGZGDriverLightingExtension.h"
#include "cIGZGDriverVertexBufferExtension.h"
#include "cIGZGSnapshotExtension.h"

#include <SDL2/SDL.h>

extern FILE *gLogFile;

#ifdef NDEBUG
#define NOTIMPL(...)
#define SIZE_CHECK(...)
#define SIZE_CHECK_RETVAL(...)
#define PRINT_PARAMETERS(...)
#define PRINT_GDRIVER_STATE(...)
#else
#define NOTIMPL()                                                \
	{                                                            \
		if (gLogFile == NULL)                                 \
			gLogFile = fopen("cGDriver.notimpl.log.txt", "w");       \
		fprintf(gLogFile, "%s: not implemented\n", __PRETTY_FUNCTION__); \
		fflush(gLogFile);                                        \
	}
#define UNEXPECTED()                                                  \
	{                                                                 \
		if (gLogFile == NULL)                                      \
			gLogFile = fopen("cGDriver.notimpl.log.txt", "w");            \
		fprintf(gLogFile, "%s:%d: Unexpected\n", __FILE__, __LINE__); \
		fflush(gLogFile);                                             \
	}
#define SIZE_CHECK(param, map)                 \
	if (param >= sizeof(map) / sizeof(map[0])) \
	{                                          \
		UNEXPECTED();                          \
		return;                                \
	}
#define SIZE_CHECK_RETVAL(param, map, ret)     \
	if (param >= sizeof(map) / sizeof(map[0])) \
	{                                          \
		UNEXPECTED();                          \
		return ret;                            \
	}

#define PRNTPARAM(x) fprintf(gLogFile, "\t%s: %d (0x%X)\n", #x, x, x);

#define PRINT_VALUES(...)                               \
/*	{                                                       \
		if (gLogFile == NULL)                            \
			gLogFile = fopen("cGDriver.notimpl.log", "w");  \
		fprintf(gLogFile, "%s Values:\n", __PRETTY_FUNCTION__); \
		FOR_EACH(PRNTPARAM, __VA_ARGS__);                 \
		fflush(gLogFile);                                   \
	}*/

#endif

namespace nSCGL
{
	class cGDriver final : public cIGZGDriver,
						   public cIGZGBufferRegionExtension,
						   public cIGZGDriverLightingExtension,
						   public cIGZGDriverVertexBufferExtension,
						   public cIGZGSnapshotExtension,
						   public cRZRefCount
	{
	private:
		enum class DriverError
		{
			OK = 0,
			OUT_OF_RANGE = 2,
			NOT_SUPPORTED = 3,
			CREATE_CONTEXT_FAIL = 6,
			INVALID_ENUM = 0x500,
			INVALID_VALUE = 0x501,

			FORCE_DWORD = 0x7FFFFFFF
		};

		enum SGLMatrixMode
		{
			MODEL_VIEW = 0,
			PROJECTION,
			TEXTURE,
			COLOR,

			NUM_MATRIX_MODES = COLOR + 1,
		};

		struct TextureStageData
		{
			uint32_t coordSrc;
			void const* textureHandle;
			bool toBeEnabled;
			bool currentlyEnabled;
		};

	private:
		SDL_Window* window;
		uint32_t glfwExtensionCount;
		const char **glfwExtensions;

	private:
		unsigned int refCount;
		DriverError lastError;

#ifndef NDEBUG
		uint32_t dbgLastError;
#endif

		std::vector<sGDMode> videoModes;
		int videoModeCount;
		int currentVideoMode;
		std::string driverInfo;

		int windowWidth, windowHeight;
		int viewportX, viewportY, viewportWidth, viewportHeight;

		uint32_t activeTextureStage; // 0x18
		uint32_t maxTextureUnits;	 // 0x28
		// We're still using the GL fixed function pipeline,
		// so we shouldn't have a high number of texture units.
		TextureStageData textureStageData[8]; // 0x2c?

		bool normalArrayEnabled, colorArrayEnabled;									  // 0x81, 0x82
		bool ambientMaterialEnabled, diffuseMaterialEnabled;						  // 0xa8, 0xa9
		float colorMultiplierR, colorMultiplierG, colorMultiplierB, colorMultiplierA; // 0x98, 0x9c, 0xa0, 0xa4

		uint32_t interleavedFormat;		// 0x84
		int32_t interleavedStride;		// 0x88
		void const *interleavedPointer; // 0x8c

        // We're not expecting to use a lot of buffer regions simultaneously, so we'll use a
        // 32-bit mask to indicate which regions are allocated and free.
        uint32_t bufferRegionFlags;

        void* deviceContext;

	private:
		void SetLastError(DriverError err);
		void SetLightingParameters();
		void ApplyTextureStages();
		int32_t InitializeVideoModeVector(void);

	public:
		cGDriver();
		virtual ~cGDriver() ;

		// We're taking the GZCLSID of the original GL driver and overriding
		// it by presenting a higher version number to the GZCOM.
		static const uint32_t kSCGLGDriverGZCLSID = 0xBADB6906;

		static bool FactoryFunctionPtr2(uint32_t riid, void **ppvObj)
		{
			cGDriver *pDriver = new cGDriver();
			bool bSucceeded = pDriver->QueryInterface(riid, ppvObj);

			if (!bSucceeded || *ppvObj == NULL)
			{
				bSucceeded = false;

				delete pDriver;
				pDriver = NULL;
			}

			return bSucceeded;
		}

	public:
		virtual bool QueryInterface(uint32_t riid, void **ppvObj) ;
		virtual uint32_t AddRef(void) ;
		virtual uint32_t Release(void) ;
		virtual bool FinalRelease(void) ;

	public:
		virtual void DrawArrays(uint32_t gdPrimType, int32_t, int32_t) ;
		virtual void DrawElements(uint32_t gdPrimType, int32_t count, uint32_t gdType, void const *indices) ;
		virtual void InterleavedArrays(uint32_t gdVertexFormat, int32_t, void const *) ;

		virtual uint32_t MakeVertexFormat(uint32_t, intptr_t gdElementTypePtr) ;
		virtual uint32_t MakeVertexFormat(uint32_t gdVertexFormat) ;
		virtual uint32_t VertexFormatStride(uint32_t gdVertexFormat) ;
		virtual uint32_t VertexFormatElementOffset(uint32_t gdVertexFormat, uint32_t gdElementType, uint32_t) ;
		virtual uint32_t VertexFormatNumElements(uint32_t gdVertexFormat, uint32_t gdElementType) ;

		virtual void Clear(uint32_t) ;
		virtual void ClearColor(float, float, float, float) ;
		virtual void ClearDepth(double) ;
		virtual void ClearStencil(int32_t) ;

		virtual void ColorMask(bool) ;
		virtual void DepthFunc(uint32_t gdTestFunc) ;
		virtual void DepthMask(bool) ;

		virtual void StencilFunc(uint32_t gdTestFunc, int32_t, uint32_t) ;
		virtual void StencilMask(uint32_t) ;
		virtual void StencilOp(uint32_t gdStencilOp, uint32_t gdStencilOp2, uint32_t gdStencilOp3) ;

		virtual void BlendFunc(uint32_t gdBlendFunc, uint32_t gdBlend) ;
		virtual void AlphaFunc(uint32_t gdTestFunc, float) ;
		virtual void ShadeModel(uint32_t gdShade) ;

		virtual void BindTexture(uint32_t gdTextureTarget, uint32_t) ;
		virtual void TexImage2D(uint32_t gdTextureTarget, int32_t, int32_t gdInternalTexFormat, int32_t, int32_t, int32_t, uint32_t gdTexFormat, uint32_t gdType, void const *) ;
		virtual void PixelStore(uint32_t gdParameter, int32_t) ;

		virtual void TexEnv(uint32_t gdTextureEnvTarget, uint32_t gdTextureEnvParamType, int32_t gdTextureEnvModeParam) ;
		virtual void TexEnv(uint32_t gdTextureEnvTarget, uint32_t gdTextureEnvParamType, float const *) ;
		virtual void TexParameter(uint32_t gdTextureTarget, uint32_t gdTextureParamType, int32_t gdTextureParam) ;

		virtual void Fog(uint32_t gdFogParamType, uint32_t gdFogParam) ;
		virtual void Fog(uint32_t gdFogParamType, float const *) ;

		virtual void ColorMultiplier(float r, float g, float b) ;
		virtual void AlphaMultiplier(float a) ;
		virtual void EnableVertexColors(bool, bool) ;

		virtual void GenTextures(int32_t, uint32_t *) ;
		virtual void DeleteTextures(int32_t, uint32_t const *) ;
		virtual bool IsTexture(uint32_t) ;
		virtual void PrioritizeTextures(int32_t, uint32_t const *, float const *) ;
		virtual bool AreTexturesResident(int32_t, uint32_t const *, bool *) ;

		virtual void MatrixMode(uint32_t gdMatrixTarget) ;
		virtual void LoadMatrix(float const *) ;
		virtual void LoadIdentity(void) ;

		virtual void Flush(void) ;
		virtual void Enable(uint32_t gdDriverState) ;
		virtual void Disable(uint32_t gdDriverState) ;
		virtual bool IsEnabled(uint32_t gdDriverState) ;

		virtual void GetBoolean(uint32_t gdParameter, bool *) ;
		virtual void GetInteger(uint32_t gdParameter, int32_t *) ;
		virtual void GetFloat(uint32_t gdParameter, float *) ;
		virtual uint32_t GetError(void) ;

		virtual void TexStage(uint32_t) ;
		virtual void TexStageCoord(uint32_t gdTexCoordSource) ;
		virtual void TexStageMatrix(float const*, uint32_t, uint32_t, uint32_t gdTexMatFlags) ;
		virtual void TexStageCombine(eGDTextureStageCombineScaleParamType gdParamType, eGDTextureStageCombineScaleParam gdParam) ;
		virtual void TexStageCombine(eGDTextureStageCombineOperandType gdParamType, eGDTextureStageCombineOperand gdBlend) ;
		virtual void TexStageCombine(eGDTextureStageCombineSourceParamType gdParamType, eGDTextureStageCombineSourceParam gdParam) ;
		virtual void TexStageCombine(eGDTextureStageCombineParamType gdParamType, eGDTextureStageCombineModeParam gdParam) ;

		virtual void SetTexture(uint32_t, uint32_t) ;
		virtual intptr_t GetTexture(uint32_t) ;
		virtual intptr_t CreateTexture(uint32_t gdInternalTexFormat, uint32_t, uint32_t, uint32_t, uint32_t gdTexHintFlags) ;
		virtual void LoadTextureLevel(uint32_t, int32_t, int32_t, int32_t, int32_t, int32_t, uint32_t gdTexFormat, uint32_t gdType, uint32_t, void const*) ;
		virtual void SetCombiner(cGDCombiner const& combiner, uint32_t) ;

		virtual uint32_t CountVideoModes(void) const ;
		virtual void GetVideoModeInfo(uint32_t dwIndex, sGDMode *gdMode) ;
		virtual void GetVideoModeInfo(sGDMode *gdMode) ;
		virtual void SetVideoMode(int32_t newModeIndex, void *, bool, bool) ;

		virtual void PolygonOffset(int32_t) ;

		virtual void BitBlt(int32_t, int32_t, int32_t, int32_t, uint32_t gdTexFormat, uint32_t gdType, void const *, bool, void const *) ;
		virtual void StretchBlt(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, uint32_t gdTexFormat, uint32_t gdType, void const *, bool, void const *) ;
		virtual void BitBltAlpha(int32_t, int32_t, int32_t, int32_t, uint32_t gdTexFormat, uint32_t gdType, void const *, bool, void const *, uint32_t) ;
		virtual void StretchBltAlpha(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, uint32_t gdTexFormat, uint32_t gdType, void const *, bool, void const *, uint32_t) ;
		virtual void BitBltAlphaModulate(int32_t, int32_t, int32_t, uint32_t gdTexFormat, uint32_t gdType, void const *, bool, void const *, uint32_t) ;
		virtual void StretchBltAlphaModulate(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, uint32_t gdTexFormat, uint32_t gdType, void const *, bool, void const *, uint32_t) ;

		virtual void SetViewport(void) ;
		virtual void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height) ;
		virtual void GetViewport(int32_t dimensions[4]) ;

		virtual char const *GetDriverInfo(void) const ;
		virtual uint32_t GetGZCLSID(void) const ;

		virtual bool Init(void) ;
		virtual bool Shutdown(void) ;
		virtual bool IsDeviceReady(void) ;
		virtual bool Punt(uint32_t, void *) ;

	public:
		virtual char const *GetVertexBufferName(uint32_t gdVertexFormat) ;
		virtual uint32_t VertexBufferType(uint32_t) ;
		virtual uint32_t MaxVertices(uint32_t) ;
		virtual uint32_t GetVertices(int32_t, bool) ;
		virtual uint32_t ContinueVertices(uint32_t, uint32_t) ;
		virtual void ReleaseVertices(uint32_t) ;
		virtual void DrawPrims(uint32_t, uint32_t gdPrimType, void *, uint32_t) ;
		virtual void DrawPrimsIndexed(uint32_t, uint32_t gdPrimType, uint32_t, uint16_t *, void *, uint32_t) ;
		virtual void Reset(void) ;

	public:
		virtual bool BufferRegionEnabled(void) ;
		virtual uint32_t NewBufferRegion(int32_t gdBufferRegionType) ;
		virtual bool DeleteBufferRegion(int32_t bufferRegion) ;
		virtual bool ReadBufferRegion(uint32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t) ;
		virtual bool DrawBufferRegion(uint32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t) ;
		virtual bool IsBufferRegion(uint32_t bufferRegion) ;
		virtual bool CanDoPartialRegionWrites(void) ;
		virtual bool CanDoOffsetReads(void) ;

		virtual bool DeleteAllBufferRegions(void) ;
		virtual cIGZBuffer *CopyColorBuffer(int32_t, int32_t, int32_t, int32_t, cIGZBuffer *) ;

	public:
		virtual void EnableLighting(bool) ;
		virtual void EnableLight(uint32_t, bool) ;
		virtual void LightModelAmbient(float, float, float, float) ;
		virtual void LightColor(uint32_t, uint32_t, float const *) ;
		virtual void LightColor(uint32_t, float const *, float const *, float const *) ;
		virtual void LightPosition(uint32_t, float const *) ;
		virtual void LightDirection(uint32_t, float const *) ;
		virtual void MaterialColor(uint32_t, float const *) ;
		virtual void MaterialColor(float const *, float const *, float const *, float const *, float) ;
	};
}
cIGZUnknown* GZCOM_CreatecSGLDriverDX7();
