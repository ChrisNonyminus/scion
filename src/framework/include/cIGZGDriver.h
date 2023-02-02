//
// Taken from https://github.com/nsgomez/scgl.
//

#ifndef SCION_CIGZGDRIVER_H
#define SCION_CIGZGDRIVER_H

#include "cIGZUnknown.h"

enum class eGDPrimType {
};
enum class eGDType {
};
enum class eGDVertexFormat {
};
enum class eGDElementType {
};
enum class eGDTestFunc {
};
enum class eGDStencilOp {
};
enum class eGDShade {
};
enum class eGDTextureTarget {
};
enum class eGDInternalTextureFormat {
};
enum class eGDTextureFormat {
};
enum class eGDParameter {
};
enum class eGDTextureEnvTarget {
};
enum class eGDTextureEnvParamType {
};
enum class eGDTextureEnvModeParam {
};
enum class eGDTextureParam {
};
enum class eGDFogParamType {
};
enum class eGDFogParam {
};
#ifdef USING_OPENGL
#include <gl/GL.h>
typedef GLenum eGDMatrixTarget;
#else
typedef uint32_t eGDMatrixTarget;
#endif


enum class eGDDriverState {
};
enum class eGDTexCoordSource {
};
enum class eGDTexMatFlags {
};
enum class eGDTextureStageCombineOperandParamType {
};
enum class eGDTextureHintFlags {
};
struct cGDCombiner {
};
struct sGDMode
{
    uint32_t index;  // 0
    uint32_t width;  // 4
    uint32_t height; // 8
    uint32_t depth;  // c // dwRGBBitCount in DDSURFACEDESC2 parlance
    uint32_t __unknown1[4];            // 10, 14, 18, 1c
    bool isFullscreen;                 // 20
    bool __unknown2;                   // 21
    bool supportsHardwareAcceleration; // 22
    bool __unknown3;                   // 23
    bool supportsTextureCompression;   // 24 // GL_ARB_texture_compression
    bool __unknown4[3];                // 25, 26, 27
    bool supportsMultitexture;         // 28 // GL_ARB_multitexture
    bool supportsTextureEnvCombine;    // 29 // GL_EXT_texture_env_combine
    bool supportsNvTextureEnvCombine4; // 2a // GL_NV_texture_env_combine4
    bool supportsFogCoord;             // 2b // GL_EXT_fog_coord
    bool supportsDxtTextures;          // 2c // GL_EXT_texture_compression_s3tc
    bool __unknown5[3];                // 2d, 2e, 2f
    bool isInitialized;                // 30
    // 34 (end of struct)
};
enum eGDCapability {
    kGDCapability_AlphaTest,
    kGDCapability_DepthTest,
    kGDCapability_StencilTest,
    kGDCapability_CullFace,
    kGDCapability_Blend,
    kGDCapability_Texture2D,
    kGDCapability_Fog,
    kGDCapability_Unused0,

    kGDNumCapabilities,
};

enum eGDTextureStageCombineScaleParamType {
    kGDTextureStageCombineScaleParamType_RGB,
    kGDTextureStageCombineScaleParamType_Alpha,

    kGDNumTextureStageCombineScaleParamTypes,
};

enum eGDTextureStageCombineScaleParam {
    kGDTextureStageCombineScaleParam_X1,
    kGDTextureStageCombineScaleParam_X2,
    kGDTextureStageCombineScaleParam_X4,

    kGDNumTextureStageCombineScaleParams,
};

enum eGDTextureStageCombineOperandType {
    kGDTextureStageCombineOperandType_Src0RGB,
    kGDTextureStageCombineOperandType_Src1RGB,
    kGDTextureStageCombineOperandType_Src2RGB,
    kGDTextureStageCombineOperandType_Src3RGB,
    kGDTextureStageCombineOperandType_Src0Alpha,
    kGDTextureStageCombineOperandType_Src1Alpha,
    kGDTextureStageCombineOperandType_Src2Alpha,
    kGDTextureStageCombineOperandType_Src3Alpha,

    kGDNumTextureStageCombineOperandTypes,
};

enum eGDBlend {
    kGDBlendTexture,
    kGDBlendPrevious,
    kGDBlendConstant,
    kGDBlendPrimaryColor,

    kGDNumBlends,
};

enum eGDTextureStageCombineSourceParamType {
    kGDTextureStageCombineSourceParamType_Operand0RGB,
    kGDTextureStageCombineSourceParamType_Operand1RGB,
    kGDTextureStageCombineSourceParamType_Operand2RGB,
    kGDTextureStageCombineSourceParamType_Operand3RGB,
    kGDTextureStageCombineSourceParamType_Operand0Alpha,
    kGDTextureStageCombineSourceParamType_Operand1Alpha,
    kGDTextureStageCombineSourceParamType_Operand2Alpha,
    kGDTextureStageCombineSourceParamType_Operand3Alpha,

    kGDNumTextureStageCombineSourceParamTypes,
};

enum eGDTextureStageCombineSourceParam {
    kGDTextureStageCombineSourceParam_Zero,
    kGDTextureStageCombineSourceParam_One,
    kGDTextureStageCombineSourceParam_SrcColor,
    kGDTextureStageCombineSourceParam_OneMinusSrcColor,
    kGDTextureStageCombineSourceParam_SrcAlpha,
    kGDTextureStageCombineSourceParam_OneMinusSrcAlpha,
    kGDTextureStageCombineSourceParam_DstAlpha,
    kGDTextureStageCombineSourceParam_OneMinusDstAlpha,
    kGDTextureStageCombineSourceParam_DstColor,
    kGDTextureStageCombineSourceParam_OneMinusDstColor,
    kGDTextureStageCombineSourceParam_SrcAlphaSaturate,

    kGDNumTextureStageCombineSourceParams,
};

enum eGDTextureStageCombineParamType {
    kGDTextureStageCombineParam_RGB,
    kGDTextureStageCombineParam_Alpha,

    kGDNumTextureStageCombineParams,
};

enum eGDTextureStageCombineModeParam {
    kGDTextureStageCombineModeParam_Replace,
    kGDTextureStageCombineModeParam_Modulate,
    kGDTextureStageCombineModeParam_Add,
    kGDTextureStageCombineModeParam_AddSigned,
    kGDTextureStageCombineModeParam_Interpolate,
    kGDTextureStageCombineModeParam_Dot3RGB,

    kGDNumTextureStageCombineModeParams,
};

struct cIGZGDriver : public cIGZUnknown {
    virtual uint32_t DrawArrays(eGDPrimType type, int32_t a3, int32_t a4) = 0;

    virtual uint32_t DrawElements(eGDPrimType type, int32_t a3, eGDType a4, const void *a5) = 0;

    virtual uint32_t InterleavedArrays(eGDVertexFormat fmt, int32_t a3, const void *a4) = 0;

    virtual uint32_t MakeVertexFormat(uint32_t a2, eGDElementType *a3) = 0;

    virtual uint32_t MakeVertexFormat(eGDVertexFormat fmt) = 0;

    virtual uint32_t VertexFormatStride(eGDVertexFormat fmt) = 0;

    virtual uint32_t VertexFormatOffset(eGDVertexFormat fmt, eGDElementType a3, uint32_t a4) = 0;

    virtual uint32_t VertexFormatNumElements(eGDVertexFormat fmt, eGDElementType a3) = 0;

    virtual uint32_t Clear(uint32_t a2) = 0;

    virtual uint32_t ClearColor(float r, float g, float b, float a) = 0;

    virtual uint32_t ClearDepth(float a2) = 0;

    virtual uint32_t ClearStencil(int32_t a2) = 0;

    virtual uint32_t ColorMask(bool a2) = 0;

    virtual uint32_t DepthFunc(eGDTestFunc func) = 0;

    virtual uint32_t DepthMask(bool a2) = 0;

    virtual uint32_t StencilFunc(eGDTestFunc func, int32_t a3, uint32_t a4) = 0;

    virtual uint32_t StencilMask(uint32_t a2) = 0;

    virtual uint32_t StencilOp(eGDStencilOp a, eGDStencilOp b, eGDStencilOp c) = 0;

    virtual uint32_t BlendFunc(eGDBlend a, eGDBlend b) = 0;

    virtual uint32_t AlphaFunc(eGDTestFunc func, float a) = 0;

    virtual uint32_t ShadeModel(eGDShade shade) = 0;

    virtual uint32_t BindTexture(eGDTextureTarget target, uint32_t a) = 0;

    virtual uint32_t TexImage2D(eGDTextureTarget target, int32_t a,
                                eGDInternalTextureFormat internalFmt, int32_t b, int32_t c, int32_t d,
                                eGDTextureFormat fmt, eGDType type, const void *data) = 0;

    virtual uint32_t PixelStore(eGDParameter param, int32_t value) = 0;

    virtual uint32_t TexEnv(eGDTextureEnvTarget target, eGDTextureEnvParamType paramType,
                            eGDTextureEnvModeParam modeParam) = 0;

    virtual uint32_t TexEnv(eGDTextureEnvTarget target, eGDTextureEnvParamType paramType,
                            const float *a) = 0;

    virtual uint32_t TexParameter(eGDTextureTarget target, eGDTextureEnvParamType paramType,
                                  eGDTextureParam param) = 0;

    virtual uint32_t Fog(eGDFogParamType paramType, eGDFogParam param) = 0;

    virtual uint32_t Fog(eGDFogParamType paramType, const float *a) = 0;

    virtual uint32_t ColorMultiplier(float r, float g, float b) = 0;

    virtual uint32_t AlphaMultiplier(float a) = 0;

    virtual uint32_t EnableVertexColors(bool a, bool b) = 0;

    virtual uint32_t GenTextures(int32_t a, uint32_t *b) = 0;

    virtual uint32_t DeleteTextures(int32_t a, const uint32_t *b) = 0;

    virtual bool IsTexture(uint32_t a) = 0;

    virtual uint32_t PrioritizeTextures(int32_t a, const uint32_t *b, const float *c) = 0;

    virtual bool AreTexturesResident(int32_t a, const uint32_t *b, bool *c) = 0;

    virtual uint32_t LoadMatrix(const float *a) = 0;

    virtual uint32_t LoadIdentity() = 0;

    virtual uint32_t Flush() = 0;

    virtual uint32_t Enable(eGDDriverState state) = 0;

    virtual uint32_t Disable(eGDDriverState state) = 0;

    virtual bool IsEnabled(eGDDriverState state) = 0;

    virtual uint32_t GetBoolean(eGDParameter param, bool *dst) = 0;

    virtual uint32_t GetInteger(eGDParameter param, int32_t *dst) = 0;

    virtual uint32_t GetFloat(eGDParameter param, float *dst) = 0;

    virtual uint32_t GetError() = 0;

    virtual uint32_t TexStage(uint32_t a) = 0;

    virtual uint32_t TexStageCoord(eGDTexCoordSource src) = 0;

    virtual uint32_t TexStageMatrix(const float *a, uint32_t b, uint32_t c, eGDTexMatFlags flags) = 0;

    virtual uint32_t TexStageCombine(eGDTextureStageCombineParamType type, eGDTextureStageCombineModeParam param) = 0;

    virtual uint32_t TexStageCombine(eGDTextureStageCombineSourceParamType type,
                                     eGDTextureStageCombineSourceParam param) = 0;

    virtual uint32_t TexStageCombine(eGDTextureStageCombineOperandParamType type, eGDBlend blend) = 0;

    virtual uint32_t TexStageCombine(eGDTextureStageCombineScaleParamType type,
                                     eGDTextureStageCombineScaleParam param) = 0;

    virtual uint32_t SetTexture(uint32_t a, uint32_t b) = 0;

    virtual uint32_t GetTexture(uint32_t a) = 0;

    virtual uint32_t CreateTexture(eGDInternalTextureFormat fmt, uint32_t a,
                                   uint32_t b, uint32_t c, eGDTextureHintFlags flags) = 0;

    virtual uint32_t LoadTextureLevel(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e,
                                      uint32_t f, eGDVertexFormat fmt, eGDType type, uint32_t g, const void *data) = 0;

    virtual uint32_t SetCombiner(const cGDCombiner &combiner, uint32_t a) = 0;

    virtual uint32_t CountVideoModes() = 0;

    virtual uint32_t GetVideoModeInfo(uint32_t a, sGDMode &dst) = 0;

    virtual uint32_t GetVideoModeInfo(sGDMode &dst) = 0;

    virtual uint32_t SetVideoMode(int32_t a, void *data, bool b, bool c) = 0;

    virtual uint32_t PolygonOffset(uint32_t a) = 0;

    virtual uint32_t
    BitBlt(long a, long b, long c, long d, eGDTextureFormat fmt, eGDType type, void const *data1, bool e,
           void const *data2) = 0;

    virtual uint32_t StretchBlt(long a, long b, long c, long d, long e, long f, eGDTextureFormat fmt,
                                eGDType type, void const *data1, bool g, void const *data2) = 0;

    virtual uint32_t
    BitBltAlpha(long a, long b, long c, long d, eGDTextureFormat fmt, eGDType type, void const *data1, bool e,
                void const *data2, uint32_t f) = 0;

    virtual uint32_t
    StretchBltAlpha(long a, long b, long c, long d, long e, long f, eGDTextureFormat fmt, eGDType type,
                    void const *data1, bool g, void const *data2,
                    uint32_t h) = 0;

    virtual uint32_t
    BitBltAlphaModulate(long a, long b, long c, long d, eGDTextureFormat fmt, eGDType type, void const *data1, bool e,
                        void const *data2, uint32_t f) = 0;

    virtual uint32_t
    StretchBltAlphaModulate(long a, long b, long c, long d, long e, long f, eGDTextureFormat fmt, eGDType type,
                            void const *data1, bool g, void const *data2, uint32_t h) = 0;

    virtual uint32_t SetViewport() = 0;

    virtual uint32_t SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;

    virtual uint32_t GetViewport() = 0;

    virtual uint32_t GetDriverInfo() = 0;

    virtual uint32_t GetGZCLSID() = 0;

    virtual uint32_t Init() = 0;

    virtual uint32_t Shutdown() = 0;

    virtual bool IsDeviceReady() = 0;

    virtual uint32_t Punt(uint32_t a, void *b) = 0;
};

static const GZIID GZIID_cIGZGDriver = 0xA4554849;

#endif //SCION_CIGZGDRIVER_H
