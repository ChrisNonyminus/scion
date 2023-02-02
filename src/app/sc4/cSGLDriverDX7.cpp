//
// Created by chris on 1/17/2023.
//
#ifdef USING_OPENGL

#include "glad/gl.h"

#ifdef USING_SDL2

#include "SDL.h"
#include "SDL_opengl.h"

#endif
#endif

#include "cSGLDriverDX7.h"
#include "RZStatics.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace nSGL {

    // This one is shared with the Textures compilation unit and can't be static.
    GLenum typeMap[16] = {
            GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT,
            GL_FLOAT, GL_DOUBLE, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_5_5_5_1, GL_RGBA,
            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_RGBA, GL_RGBA
    };

    static GLenum drawModeMap[8] = {GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_POINTS, GL_LINES,
                                    GL_LINE_STRIP, GL_QUADS, GL_QUAD_STRIP};
    static GLenum glFuncMap[8] = {GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL,
                                  GL_ALWAYS};
    static GLenum capabilityMap[8] = {GL_ALPHA_TEST, GL_DEPTH_TEST, GL_STENCIL_TEST, GL_CULL_FACE, GL_BLEND,
                                      GL_TEXTURE_2D, GL_FOG, 0};
    static GLenum fogParamTypeMap[] = {GL_FOG_MODE, GL_FOG_COLOR, GL_FOG_DENSITY, GL_FOG_START, GL_FOG_END,
                                       GL_FOG_COORD_SRC};

    static GLenum glBlendMap[11] = {
            GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR,
            GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
            GL_ONE_MINUS_DST_ALPHA, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
            GL_SRC_ALPHA_SATURATE
    };
}

nSGL::cGDriver::cGDriver() {

    field_1E0.index = -1;





    // initialize sD3DIdentMat
#if defined(USING_OPENGL)
    sD3DIdentMat = glm::mat4(1.0f);
#else

#endif




}

uint32_t nSGL::cGDriver::DrawArrays(eGDPrimType type, int32_t a3, int32_t a4) {
    return 0;
}

uint32_t nSGL::cGDriver::DrawElements(eGDPrimType type, int32_t a3, eGDType a4, const void *a5) {
    return 0;
}

uint32_t nSGL::cGDriver::InterleavedArrays(eGDVertexFormat fmt, int32_t a3, const void *a4) {
    return 0;
}

uint32_t nSGL::cGDriver::MakeVertexFormat(uint32_t a2, eGDElementType *a3) {
    return 0;
}

uint32_t nSGL::cGDriver::MakeVertexFormat(eGDVertexFormat fmt) {
    return 0;
}

uint32_t nSGL::cGDriver::VertexFormatStride(eGDVertexFormat fmt) {
    return 0;
}

uint32_t nSGL::cGDriver::VertexFormatOffset(eGDVertexFormat fmt, eGDElementType a3, uint32_t a4) {
    return 0;
}

uint32_t nSGL::cGDriver::VertexFormatNumElements(eGDVertexFormat fmt, eGDElementType a3) {
    return 0;
}

uint32_t nSGL::cGDriver::Clear(uint32_t a2) {
    return 0;
}

uint32_t nSGL::cGDriver::ClearColor(float r, float g, float b, float a) {
    return 0;
}

uint32_t nSGL::cGDriver::ClearDepth(float a2) {
    return 0;
}

uint32_t nSGL::cGDriver::ClearStencil(int32_t a2) {
    return 0;
}

uint32_t nSGL::cGDriver::ColorMask(bool a2) {
    return 0;
}

uint32_t nSGL::cGDriver::DepthFunc(eGDTestFunc func) {
    return 0;
}

uint32_t nSGL::cGDriver::DepthMask(bool a2) {
    return 0;
}

uint32_t nSGL::cGDriver::StencilFunc(eGDTestFunc func, int32_t a3, uint32_t a4) {
    return 0;
}

uint32_t nSGL::cGDriver::StencilMask(uint32_t a2) {
    return 0;
}

uint32_t nSGL::cGDriver::StencilOp(eGDStencilOp a, eGDStencilOp b, eGDStencilOp c) {
    return 0;
}

uint32_t nSGL::cGDriver::BlendFunc(eGDBlend a, eGDBlend b) {
    return 0;
}

uint32_t nSGL::cGDriver::AlphaFunc(eGDTestFunc func, float a) {
    return 0;
}

uint32_t nSGL::cGDriver::ShadeModel(eGDShade shade) {
    return 0;
}

uint32_t nSGL::cGDriver::BindTexture(eGDTextureTarget target, uint32_t a) {
    return 0;
}

uint32_t
nSGL::cGDriver::TexImage2D(eGDTextureTarget target, int32_t a, eGDInternalTextureFormat internalFmt, int32_t b,
                           int32_t c, int32_t d, eGDTextureFormat fmt, eGDType type, const void *data) {
    return 0;
}

uint32_t nSGL::cGDriver::PixelStore(eGDParameter param, int32_t value) {
    return 0;
}

uint32_t nSGL::cGDriver::TexEnv(eGDTextureEnvTarget target, eGDTextureEnvParamType paramType,
                                eGDTextureEnvModeParam modeParam) {
    return 0;
}

uint32_t nSGL::cGDriver::TexEnv(eGDTextureEnvTarget target, eGDTextureEnvParamType paramType, const float *a) {
    return 0;
}

uint32_t
nSGL::cGDriver::TexParameter(eGDTextureTarget target, eGDTextureEnvParamType paramType, eGDTextureParam param) {
    return 0;
}

uint32_t nSGL::cGDriver::Fog(eGDFogParamType paramType, eGDFogParam param) {
    return 0;
}

uint32_t nSGL::cGDriver::Fog(eGDFogParamType paramType, const float *a) {
    return 0;
}

uint32_t nSGL::cGDriver::ColorMultiplier(float r, float g, float b) {
    return 0;
}

uint32_t nSGL::cGDriver::AlphaMultiplier(float a) {
    return 0;
}

uint32_t nSGL::cGDriver::EnableVertexColors(bool a, bool b) {
    return 0;
}

uint32_t nSGL::cGDriver::GenTextures(int32_t a, uint32_t *b) {
    return 0;
}

uint32_t nSGL::cGDriver::DeleteTextures(int32_t a, const uint32_t *b) {
    return 0;
}

bool nSGL::cGDriver::IsTexture(uint32_t a) {
    return false;
}

uint32_t nSGL::cGDriver::PrioritizeTextures(int32_t a, const uint32_t *b, const float *c) {
    return 0;
}

bool nSGL::cGDriver::AreTexturesResident(int32_t a, const uint32_t *b, bool *c) {
    return false;
}

uint32_t nSGL::cGDriver::LoadMatrix(const float *a) {
    glLoadMatrixf(a);
}

uint32_t nSGL::cGDriver::LoadIdentity() {
    return 0;
}

uint32_t nSGL::cGDriver::Flush() {
    return 0;
}

uint32_t nSGL::cGDriver::Enable(eGDDriverState state) {
    return 0;
}

uint32_t nSGL::cGDriver::Disable(eGDDriverState state) {
    return 0;
}

bool nSGL::cGDriver::IsEnabled(eGDDriverState state) {
    return false;
}

uint32_t nSGL::cGDriver::GetBoolean(eGDParameter param, bool *dst) {
    return 0;
}

uint32_t nSGL::cGDriver::GetInteger(eGDParameter param, int32_t *dst) {
    return 0;
}

uint32_t nSGL::cGDriver::GetFloat(eGDParameter param, float *dst) {
    return 0;
}

uint32_t nSGL::cGDriver::GetError() {
    return 0;
}

uint32_t nSGL::cGDriver::TexStage(uint32_t a) {
    return 0;
}

uint32_t nSGL::cGDriver::TexStageCoord(eGDTexCoordSource src) {
    return 0;
}

uint32_t nSGL::cGDriver::TexStageMatrix(const float *a, uint32_t b, uint32_t c, eGDTexMatFlags flags) {
    return 0;
}

uint32_t
nSGL::cGDriver::TexStageCombine(eGDTextureStageCombineParamType type, eGDTextureStageCombineModeParam param) {
    return 0;
}

uint32_t nSGL::cGDriver::TexStageCombine(eGDTextureStageCombineSourceParamType type,
                                         eGDTextureStageCombineSourceParam param) {
    return 0;
}

uint32_t nSGL::cGDriver::TexStageCombine(eGDTextureStageCombineOperandParamType type, eGDBlend blend) {
    return 0;
}

uint32_t
nSGL::cGDriver::TexStageCombine(eGDTextureStageCombineScaleParamType type, eGDTextureStageCombineScaleParam param) {
    return 0;
}

uint32_t nSGL::cGDriver::SetTexture(uint32_t a, uint32_t b) {
    return 0;
}

uint32_t nSGL::cGDriver::GetTexture(uint32_t a) {
    return 0;
}

uint32_t nSGL::cGDriver::CreateTexture(eGDInternalTextureFormat fmt, uint32_t a, uint32_t b, uint32_t c,
                                       eGDTextureHintFlags flags) {
    return 0;
}

uint32_t nSGL::cGDriver::LoadTextureLevel(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f,
                                          eGDVertexFormat fmt, eGDType type, uint32_t g, const void *data) {
    return 0;
}

uint32_t nSGL::cGDriver::SetCombiner(const cGDCombiner &combiner, uint32_t a) {
    return 0;
}

uint32_t nSGL::cGDriver::CountVideoModes() {
    return 0;
}

uint32_t nSGL::cGDriver::GetVideoModeInfo(uint32_t a, sGDMode &dst) {
    return 0;
}

uint32_t nSGL::cGDriver::GetVideoModeInfo(sGDMode &dst) {
    return 0;
}

uint32_t nSGL::cGDriver::SetVideoMode(int32_t a, void *data, bool b, bool c) {
    this->EndScene();

    if (a != -1) {
        field_1E0.index = a;

        field_1E0 = m_modes[a];
    }
    else {
        field_1E0 = *(sGDMode *) data;
    }

    if (Display::SetupDisplay(!bFullscreen, field_1E0.width, field_1E0.height)) {
        m_window = Display::GetMainWindow();

        if (!this->SetupViewport()) {
            this->DestroyViewport();
            m_window = nullptr;
        }
    }
    if (!m_window) {
        return 0;
    }
}

uint32_t nSGL::cGDriver::PolygonOffset(uint32_t a) {
    return 0;
}

uint32_t
nSGL::cGDriver::BitBlt(long a, long b, long c, long d, eGDTextureFormat fmt, eGDType type, const void *data1, bool e,
                       const void *data2) {
    return 0;
}

uint32_t
nSGL::cGDriver::StretchBlt(long a, long b, long c, long d, long e, long f, eGDTextureFormat fmt, eGDType type,
                           const void *data1, bool g, const void *data2) {
    return 0;
}

uint32_t
nSGL::cGDriver::BitBltAlpha(long a, long b, long c, long d, eGDTextureFormat fmt, eGDType type, const void *data1,
                            bool e, const void *data2, uint32_t f) {
    return 0;
}

uint32_t
nSGL::cGDriver::StretchBltAlpha(long a, long b, long c, long d, long e, long f, eGDTextureFormat fmt, eGDType type,
                                const void *data1, bool g, const void *data2, uint32_t h) {
    return 0;
}

uint32_t nSGL::cGDriver::BitBltAlphaModulate(long a, long b, long c, long d, eGDTextureFormat fmt, eGDType type,
                                             const void *data1, bool e, const void *data2, uint32_t f) {
    return 0;
}

uint32_t
nSGL::cGDriver::StretchBltAlphaModulate(long a, long b, long c, long d, long e, long f, eGDTextureFormat fmt,
                                        eGDType type, const void *data1, bool g, const void *data2, uint32_t h) {
    return 0;
}

uint32_t nSGL::cGDriver::SetViewport() {
    return 0;
}

uint32_t nSGL::cGDriver::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    return 0;
}

uint32_t nSGL::cGDriver::GetViewport() {
    return 0;
}

uint32_t nSGL::cGDriver::GetDriverInfo() {
    return 0;
}

uint32_t nSGL::cGDriver::GetGZCLSID() {
    return 0;
}

uint32_t nSGL::cGDriver::Init() {
    field_11C = 0;
    if (bInitialized) return 0;
    bInitialized = true;
    field_120 = 0;
    field_124 = 0;
    field_138 = 7;
    field_1E0.index = -1;
    this->InitViewport();
}

uint32_t nSGL::cGDriver::Shutdown() {
    return 0;
}

bool nSGL::cGDriver::IsDeviceReady() {
    return false;
}

uint32_t nSGL::cGDriver::Punt(uint32_t a, void *b) {
    return 0;
}

bool nSGL::cGDriver::QueryInterface(GZIID iid, void **outPtr) {
    return false;
}

uint32_t nSGL::cGDriver::AddRef() {
    return 0;
}

uint32_t nSGL::cGDriver::Release() {
    return 0;
}

void nSGL::cGDriver::InitViewport() {
    field_1D4 = 0;
}

void nSGL::cGDriver::EndScene() {
    if (field_260 && m_context) {
#ifdef USING_OPENGL
        glFlush();


#endif
    }
}

bool nSGL::cGDriver::SetupViewport() {
    EndScene();
    if (!this->Initialize3DEnvironment())
        return false;
    this->SetViewport();
    sGDMode mode;
    mode = m_modes[field_1E0.index];
    field_1E0 = mode;
    return true;
}

void nSGL::cGDriver::DestroyViewport() {
    EndScene();
    this->Cleanup3DEnvironment();
    Display::ReleaseDisplay();
}

bool nSGL::cGDriver::Initialize3DEnvironment() {
    uint8_t a2;
    RZGLContext context = Display::CreateContext(0, a2);
    if (!context) {
        return false;
    }
    m_context = context;

    // Initialize GLAD
#ifdef USING_SDL2
    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);

    printf("OpenGL version loaded: %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
#endif

    // Set up the viewport
    glViewport(0, 0, field_1E0.width, field_1E0.height);

    static glm::mat4 matIdent = glm::mat4(1.0f);

    // Set up the projection matrix
    glm::mat4 matProj = glm::perspective(glm::radians(45.0f), (float) field_1E0.width / (float) field_1E0.height, 0.1f,
                                         100.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(matProj));

    // Set up the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(matIdent));

    // Set up the texture matrix
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(glm::value_ptr(matIdent));

    // Set up the color matrix
    glMatrixMode(GL_COLOR);
    glLoadMatrixf(glm::value_ptr(matIdent));


    return true;
}

void nSGL::cGDriver::Cleanup3DEnvironment() {
    if (m_context) {
        Display::ReleaseContext(m_context);
        m_context = nullptr;
    }


}

cIGZUnknown *GZCOM_CreatecSGLDriverDX7() {
    return new nSGL::cGDriver();
}
