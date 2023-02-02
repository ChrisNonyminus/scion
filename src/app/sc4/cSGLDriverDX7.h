//
// Created by chris on 1/17/2023.
//

#ifndef SCION_CSGLDRIVERDX7_H
#define SCION_CSGLDRIVERDX7_H

#include "cIGZGDriver.h"

#ifdef USING_OPENGL
// use OpenGL instead of DirectX
// use glm too
#include "glm/glm.hpp"
#include "cRZAutoRefCount.h"

typedef glm::mat4x4 GZMatrix;
#ifdef _WIN32

#include <GL/gl.h>
#include <GL/glu.h>

#elif defined(__linux__)
#include <GL/gl.h>
#include <GL/glu.h>
#elif defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else // Unknown platform
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#ifdef USING_SDL2

#include <SDL_opengl.h>
#include <vector>

#endif
#else
#include <d3d9.h>
#include <d3dx9.h>
typedef D3DXMATRIX GZMatrix;
#endif

#include "RZStatics.h"


inline uint32_t GZCOM_cSGLDriverDX7CLSID() { return 0xBADB6906; }

namespace nSGL {
    struct cTextureStageData {
        uint32_t coordSrc;
        uint32_t unknown;
        bool enabled;
    };

    class cTextureManager : public cIGZUnknown {
    };

    class cPrimitiveManager : public cIGZUnknown {
    };

    class cGDriver : public cIGZGDriver {
    public:
        cGDriver();


        static GZMatrix sD3DIdentMat;


        struct BufferRegionData {
        };
        struct D3DLight {
        };


        uint32_t DrawArrays(eGDPrimType type, int32_t a3, int32_t a4) override;

        uint32_t DrawElements(eGDPrimType type, int32_t a3, eGDType a4, const void *a5) override;

        uint32_t InterleavedArrays(eGDVertexFormat fmt, int32_t a3, const void *a4) override;

        uint32_t MakeVertexFormat(uint32_t a2, eGDElementType *a3) override;

        uint32_t MakeVertexFormat(eGDVertexFormat fmt) override;

        uint32_t VertexFormatStride(eGDVertexFormat fmt) override;

        uint32_t VertexFormatOffset(eGDVertexFormat fmt, eGDElementType a3, uint32_t a4) override;

        uint32_t VertexFormatNumElements(eGDVertexFormat fmt, eGDElementType a3) override;

        uint32_t Clear(uint32_t a2) override;

        uint32_t ClearColor(float r, float g, float b, float a) override;

        uint32_t ClearDepth(float a2) override;

        uint32_t ClearStencil(int32_t a2) override;

        uint32_t ColorMask(bool a2) override;

        uint32_t DepthFunc(eGDTestFunc func) override;

        uint32_t DepthMask(bool a2) override;

        uint32_t StencilFunc(eGDTestFunc func, int32_t a3, uint32_t a4) override;

        uint32_t StencilMask(uint32_t a2) override;

        uint32_t StencilOp(eGDStencilOp a, eGDStencilOp b, eGDStencilOp c) override;

        uint32_t BlendFunc(eGDBlend a, eGDBlend b) override;

        uint32_t AlphaFunc(eGDTestFunc func, float a) override;

        uint32_t ShadeModel(eGDShade shade) override;

        uint32_t BindTexture(eGDTextureTarget target, uint32_t a) override;

        uint32_t
        TexImage2D(eGDTextureTarget target, int32_t a, eGDInternalTextureFormat internalFmt, int32_t b, int32_t c,
                   int32_t d, eGDTextureFormat fmt, eGDType type, const void *data) override;

        uint32_t PixelStore(eGDParameter param, int32_t value) override;

        uint32_t
        TexEnv(eGDTextureEnvTarget target, eGDTextureEnvParamType paramType, eGDTextureEnvModeParam modeParam) override;

        uint32_t TexEnv(eGDTextureEnvTarget target, eGDTextureEnvParamType paramType, const float *a) override;

        uint32_t
        TexParameter(eGDTextureTarget target, eGDTextureEnvParamType paramType, eGDTextureParam param) override;

        uint32_t Fog(eGDFogParamType paramType, eGDFogParam param) override;

        uint32_t Fog(eGDFogParamType paramType, const float *a) override;

        uint32_t ColorMultiplier(float r, float g, float b) override;

        uint32_t AlphaMultiplier(float a) override;

        uint32_t EnableVertexColors(bool a, bool b) override;

        uint32_t GenTextures(int32_t a, uint32_t *b) override;

        uint32_t DeleteTextures(int32_t a, const uint32_t *b) override;

        bool IsTexture(uint32_t a) override;

        uint32_t PrioritizeTextures(int32_t a, const uint32_t *b, const float *c) override;

        bool AreTexturesResident(int32_t a, const uint32_t *b, bool *c) override;

        uint32_t LoadMatrix(const float *a) override;

        uint32_t LoadIdentity() override;

        uint32_t Flush() override;

        uint32_t Enable(eGDDriverState state) override;

        uint32_t Disable(eGDDriverState state) override;

        bool IsEnabled(eGDDriverState state) override;

        uint32_t GetBoolean(eGDParameter param, bool *dst) override;

        uint32_t GetInteger(eGDParameter param, int32_t *dst) override;

        uint32_t GetFloat(eGDParameter param, float *dst) override;

        uint32_t GetError() override;

        uint32_t TexStage(uint32_t a) override;

        uint32_t TexStageCoord(eGDTexCoordSource src) override;

        uint32_t TexStageMatrix(const float *a, uint32_t b, uint32_t c, eGDTexMatFlags flags) override;

        uint32_t TexStageCombine(eGDTextureStageCombineParamType type, eGDTextureStageCombineModeParam param) override;

        uint32_t
        TexStageCombine(eGDTextureStageCombineSourceParamType type, eGDTextureStageCombineSourceParam param) override;

        uint32_t TexStageCombine(eGDTextureStageCombineOperandParamType type, eGDBlend blend) override;

        uint32_t
        TexStageCombine(eGDTextureStageCombineScaleParamType type, eGDTextureStageCombineScaleParam param) override;

        uint32_t SetTexture(uint32_t a, uint32_t b) override;

        uint32_t GetTexture(uint32_t a) override;

        uint32_t CreateTexture(eGDInternalTextureFormat fmt, uint32_t a, uint32_t b, uint32_t c,
                               eGDTextureHintFlags flags) override;

        uint32_t
        LoadTextureLevel(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, eGDVertexFormat fmt,
                         eGDType type, uint32_t g, const void *data) override;

        uint32_t SetCombiner(const cGDCombiner &combiner, uint32_t a) override;

        uint32_t CountVideoModes() override;

        uint32_t GetVideoModeInfo(uint32_t a, sGDMode &dst) override;

        uint32_t GetVideoModeInfo(sGDMode &dst) override;

        uint32_t SetVideoMode(int32_t a, void *data, bool b, bool c) override;

        uint32_t PolygonOffset(uint32_t a) override;

        uint32_t BitBlt(long a, long b, long c, long d, eGDTextureFormat fmt, eGDType type, const void *data1, bool e,
                        const void *data2) override;

        uint32_t StretchBlt(long a, long b, long c, long d, long e, long f, eGDTextureFormat fmt, eGDType type,
                            const void *data1, bool g, const void *data2) override;

        uint32_t
        BitBltAlpha(long a, long b, long c, long d, eGDTextureFormat fmt, eGDType type, const void *data1, bool e,
                    const void *data2, uint32_t f) override;

        uint32_t StretchBltAlpha(long a, long b, long c, long d, long e, long f, eGDTextureFormat fmt, eGDType type,
                                 const void *data1, bool g, const void *data2, uint32_t h) override;

        uint32_t
        BitBltAlphaModulate(long a, long b, long c, long d, eGDTextureFormat fmt, eGDType type, const void *data1,
                            bool e, const void *data2, uint32_t f) override;

        uint32_t
        StretchBltAlphaModulate(long a, long b, long c, long d, long e, long f, eGDTextureFormat fmt, eGDType type,
                                const void *data1, bool g, const void *data2, uint32_t h) override;

        uint32_t SetViewport() override;

        uint32_t SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) override;

        uint32_t GetViewport() override;

        uint32_t GetDriverInfo() override;

        uint32_t GetGZCLSID() override;

        uint32_t Init() override;

        uint32_t Shutdown() override;

        bool IsDeviceReady() override;

        uint32_t Punt(uint32_t a, void *b) override;

        bool QueryInterface(GZIID iid, void **outPtr) override;

        uint32_t AddRef() override;

        uint32_t Release() override;

    private:
        std::vector<sGDMode> m_modes;
        std::vector<cTextureStageData> m_textureStages;
        std::vector<BufferRegionData> m_bufferRegions;
        std::vector<D3DLight> m_lights;

        cRZAutoRefCount<cTextureManager> m_textureManager;
        cRZAutoRefCount<cPrimitiveManager> m_primitiveManager;

        eGDMatrixTarget m_matrixMode;

        RZWindow m_window;
        RZGLContext m_context;
        int field_11C;

        bool bInitialized;
        int iViewportX, iViewportY, iViewportW, iViewportH;
        int field_120, field_124, field_138, field_1D4;
        sGDMode field_1E0;
        bool bFullscreen;
        int field_260;


        void InitViewport();

        void EndScene();

        bool SetupViewport();

        void DestroyViewport();

        bool Initialize3DEnvironment();

        void Cleanup3DEnvironment();
    };
}

cIGZUnknown *GZCOM_CreatecSGLDriverDX7();

#endif //SCION_CSGLDRIVERDX7_H
