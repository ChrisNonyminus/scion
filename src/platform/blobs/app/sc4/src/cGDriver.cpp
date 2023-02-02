#include <string>

#define GLAD_GL_IMPLEMENTATION
#include "glad/gl.h"
#include "cGDriver.h"
#include <SDL2/SDL_opengl.h>


#include "cGDCombiner.h"

#ifdef NDEBUG
#define DBGLOGERR()
#else
#define DBGLOGERR() dbgLastError = glGetError();
#endif

#include "cGZGraphicSystem.h"

FILE* gLogFile = NULL;
cIGZGBufferRegionExtension::~cIGZGBufferRegionExtension() { }
cIGZGDriverVertexBufferExtension::~cIGZGDriverVertexBufferExtension() { }

/*static_assert(sizeof(sGDMode) == 56U);
static_assert(offsetof(sGDMode, fullscreen) == 0x20);
static_assert(offsetof(sGDMode, is3DAccelerated) == 0x22);
static_assert(offsetof(sGDMode, _unknownFuncPtr) == 0x34);*/

namespace nSCGL
{
	// This one is shared with the Textures compilation unit and can't be static.
	GLenum typeMap[16] = {
		GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT,
		GL_FLOAT, GL_DOUBLE, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_5_5_5_1, GL_RGBA,
		GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_RGBA, GL_RGBA
	};

	static GLenum drawModeMap[8] = { GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_QUADS, GL_QUAD_STRIP };
	static GLenum glFuncMap[8] = { GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS };
	static GLenum capabilityMap[8] = { GL_ALPHA_TEST, GL_DEPTH_TEST, GL_STENCIL_TEST, GL_CULL_FACE, GL_BLEND, GL_TEXTURE_2D, GL_FOG, 0 };
	static GLenum fogParamTypeMap[] = { GL_FOG_MODE, GL_FOG_COLOR, GL_FOG_DENSITY, GL_FOG_START, GL_FOG_END, GL_FOG_COORD_SRC };

	static GLenum glBlendMap[11] = {
		GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
		GL_SRC_ALPHA_SATURATE
	};

	cGDriver::cGDriver() :
		window(NULL),
		glfwExtensionCount(),
		glfwExtensions(),
		lastError(DriverError::OK),
#ifndef NDEBUG
		dbgLastError(GL_NO_ERROR),
#endif
		currentVideoMode(-1),
		driverInfo("Maxis 3D GDriver\nOpenGL\n3.0\n"),
		videoModeCount(0),
		refCount(0),
		windowWidth(0),
		windowHeight(0),
		viewportX(0),
		viewportY(0),
		viewportWidth(0),
		viewportHeight(0),
		colorMultiplierR(0.0f),
		colorMultiplierG(0.0f),
		colorMultiplierB(0.0f),
		colorMultiplierA(0.0f),
		interleavedFormat(0),
		interleavedStride(0),
		interleavedPointer(NULL),
		normalArrayEnabled(false),
		colorArrayEnabled(false),
		ambientMaterialEnabled(false),
		diffuseMaterialEnabled(false),
		activeTextureStage(0),
		maxTextureUnits(0),
		textureStageData(),
		deviceContext(NULL),
		bufferRegionFlags(0)
	{
	}

	cGDriver::~cGDriver() {
	}

	void cGDriver::DrawArrays(GLenum gdMode, GLint first, GLsizei count) {
		SIZE_CHECK(gdMode, drawModeMap);

		GLenum mode = drawModeMap[gdMode];

		ApplyTextureStages();
		glDrawArrays(mode, first, count);
	}

	void cGDriver::DrawElements(GLenum gdMode, GLsizei count, GLenum gdType, void const* indices) {
		SIZE_CHECK(gdMode, drawModeMap);
		SIZE_CHECK(gdType, typeMap);

		GLenum mode = drawModeMap[gdMode];
		GLenum type = typeMap[gdType];

		ApplyTextureStages();
		glDrawElements(mode, count, type, indices);
	}

	void cGDriver::InterleavedArrays(GLenum format, GLsizei stride, void const* pointer) {
		if (stride == 0) {
			stride = VertexFormatStride(format);
		}

		int normalLength = VertexFormatNumElements(format, 3);
		int colorLength = VertexFormatNumElements(format, 5);

		float const* fPointer = reinterpret_cast<float const*>(pointer);
		glVertexPointer(3, GL_FLOAT, stride, pointer);

		if (normalLength == 0) {
			if (normalArrayEnabled) {
				glDisableClientState(GL_NORMAL_ARRAY);
				normalArrayEnabled = false;
			}
		}
		else {
			if (!normalArrayEnabled) {
				glEnableClientState(GL_NORMAL_ARRAY);
				normalArrayEnabled = true;
			}

			int normalOffset = VertexFormatElementOffset(format, 3, 0);
			glNormalPointer(GL_FLOAT, stride, reinterpret_cast<uint8_t const*>(pointer) + normalOffset);
		}

		if (colorLength == 0) {
			if (colorArrayEnabled) {
				glDisableClientState(GL_COLOR_ARRAY);
				colorArrayEnabled = false;
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		else {
			if (!colorArrayEnabled) {
				glEnableClientState(GL_COLOR_ARRAY);
				colorArrayEnabled = true;
			}

			int colorOffset = VertexFormatElementOffset(format, 5, 0);

			// GPU must implement GL_ARB_vertex_array_bgra or GL_EXT_vertex_array_bgra for this to work.
			// These extensions did not exist when SimCity 4 was released, so their workaround was to
			// use the CPU to swap the order of color components. That's slow - let's never do that.
			glColorPointer(GL_BGRA, GL_UNSIGNED_BYTE, stride, reinterpret_cast<uint8_t const*>(pointer) + colorOffset);
		}

		interleavedFormat = format;
		interleavedStride = stride;
		interleavedPointer = pointer;
	}

	uint32_t cGDriver::MakeVertexFormat(uint32_t, intptr_t gdElementTypePtr) {
		NOTIMPL();
		return -1;
	}

	uint32_t cGDriver::MakeVertexFormat(uint32_t gdVertexFormat) {
		// nRZSimGL::PackStandardVertexFormat(eGDVertexFormat) -> eGDVertexFormat
		static uint32_t formatToPackedFormatMap[] = {
			0x00000000, 0x80000101, 0x80004001, 0x80008001,
			0x00000000, 0x00000000, 0x00000000, 0x00000000,
			0x00000000, 0x00000000, 0x80004101, 0x80008101,
			0x00000000, 0x00000000, 0x00000000, 0x00000000,
			0x00000000, 0x00000000, 0x00000000, 0x00000000,
			0x00000000, 0x00000000, 0x00000000, 0x00000000,
			0x00000000, 0x00000000, 0x00000000, 0x00000000,
			0x00000000, 0x00000000, 0x00000000, 0x00000000,
			0x80000001, 0x80000041, 0x80000141, 0x80004041,
			0x80008041, 0x80004141, 0x80008141,
		};

		static constexpr size_t formatMapLength = sizeof(formatToPackedFormatMap) / sizeof(uint32_t);
		if (gdVertexFormat >= formatMapLength) {
			return gdVertexFormat;
		}

		return formatToPackedFormatMap[gdVertexFormat];
	}

	uint32_t cGDriver::VertexFormatStride(uint32_t gdVertexFormat) {
		switch (gdVertexFormat) {
		case 0x01:
			return 0x10;
		case 0x02:
			return 0x14;
		case 0x03:
		case 0x22:
			return 0x1c;
		case 0x0a:
		case 0x21:
			return 0x18;
		case 0x0b:
		case 0x23:
			return 0x20;
		case 0x20:
			return 0xc;
		case 0x24:
			return 0x28;
		case 0x25:
			return 0x24;
		case 0x26:
			return 0x2c;
		}

		if (gdVertexFormat < 0x80000000) {
			gdVertexFormat = MakeVertexFormat(gdVertexFormat);
		}

		gdVertexFormat &= 0x7fffffff;
		uint32_t stride = ((gdVertexFormat >> 10) & 0xf)
			+ (((gdVertexFormat >> 14) & 0xf) * 2)
			+ ((gdVertexFormat >> 8) & 0x3)
			+ ((gdVertexFormat & 3) * 3);

		stride *= sizeof(GLfloat);
		if ((gdVertexFormat & 0xf) != 0) {
			uint32_t addlStride = ((gdVertexFormat >> 18) & 0xf)
				+ (((gdVertexFormat >> 6) & 1) * 3)
				+ (((gdVertexFormat >> 22) & 0xf) * 4)
				+ ((gdVertexFormat >> 2) & 7)
				+ ((gdVertexFormat >> 7) & 1)
				+ ((gdVertexFormat >> 5) & 1);

			addlStride *= 4;
			stride += addlStride;
		}

		return stride;
	}

	uint32_t cGDriver::VertexFormatElementOffset(uint32_t gdVertexFormat, uint32_t gdElementType, uint32_t count) {
		static uint32_t elementTypeOffsetMap[] = { 12, 4, 4, 12, 4, 4, 4, 8, 12, 16 };
		static uint32_t elementTypeVertexFormatMap[] = { 0xf, 0x0, 0x3, 0x1f, 0x3f, 0x7f, 0xff, 0x3ff, 0x3fff, 0x3ffff, 0x3fffff };

		uint32_t offset = elementTypeOffsetMap[gdElementType] * count;
		if (gdVertexFormat < 0x80000000) {
			gdVertexFormat = MakeVertexFormat(gdVertexFormat);
		}

		if (gdElementType > 0) {
			offset += VertexFormatStride((elementTypeVertexFormatMap[gdElementType] | 0x80000000) & gdVertexFormat);
		}

		return offset;
	}

	uint32_t cGDriver::VertexFormatNumElements(uint32_t gdVertexFormat, uint32_t gdElementType) {
		static uint32_t elementTypeShiftFactor[] = { 0, 2, 5, 6, 7, 8, 10, 14, 18, 22 };
		static uint32_t elementTypeMask[] = { 0x3, 0x7, 0x1, 0x1, 0x1, 0x3, 0xf, 0xf, 0xf };

		if (gdVertexFormat < 0x80000000) {
			gdVertexFormat = MakeVertexFormat(gdVertexFormat);
		}

		return (gdVertexFormat >> elementTypeShiftFactor[gdElementType]) & elementTypeMask[gdElementType];
	}

	void cGDriver::Clear(GLbitfield mask) {
		GLbitfield glMask = 0;
		glMask  = (mask & 0x1000) >> 4; // GL_DEPTH_BUFFER_BIT   (0x100)
		glMask |= (mask & 0x2000) >> 3; // GL_STENCIL_BUFFER_BIT (0x400)
		glMask |= (mask & 0x4000);      // GL_COLOR_BUFFER_BIT   (0x4000)
		glClear(glMask);
	}

	void cGDriver::ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
		//glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClearColor(red, green, blue, alpha);
	}

	void cGDriver::ClearDepth(GLclampd depth) {
		glClearDepth(depth);
	}

	void cGDriver::ClearStencil(GLint s) {
		glClearStencil(s);
	}

	void cGDriver::ColorMask(bool flag) {
		glColorMask(flag, flag, flag, flag);
	}

	void cGDriver::DepthFunc(GLenum gdFunc) {
		SIZE_CHECK(gdFunc, glFuncMap);
		glDepthFunc(glFuncMap[gdFunc]);
	}

	void cGDriver::DepthMask(bool flag) {
		glFlush();
		glDepthMask(flag);
	}

	void cGDriver::StencilFunc(GLenum gdFunc, GLint ref, GLuint mask) {
		SIZE_CHECK(gdFunc, glFuncMap);

		if (videoModes[currentVideoMode].supportsStencilBuffer) {
			glStencilFunc(glFuncMap[gdFunc], ref, mask);
		}
	}

	void cGDriver::StencilMask(GLuint mask) {
		if (videoModes[currentVideoMode].supportsStencilBuffer) {
			glStencilMask(mask);
		}
	}

	void cGDriver::StencilOp(GLenum fail, GLenum zfail, GLenum zpass) {
		static GLenum glStencilMap[] = { GL_KEEP, GL_REPLACE, GL_INCR, GL_DECR, GL_INVERT };
		SIZE_CHECK(fail, glStencilMap);
		SIZE_CHECK(zfail, glStencilMap);
		SIZE_CHECK(zpass, glStencilMap);

		if (videoModes[currentVideoMode].supportsStencilBuffer) {
			glStencilOp(glStencilMap[fail], glStencilMap[zfail], glStencilMap[zpass]);
		}
	}

	void cGDriver::BlendFunc(GLenum sfactor, GLenum dfactor) {
		SIZE_CHECK(sfactor, glBlendMap);
		SIZE_CHECK(dfactor, glBlendMap);

		glBlendFunc(glBlendMap[sfactor], glBlendMap[dfactor]);
	}

	void cGDriver::AlphaFunc(GLenum func, GLclampf ref) {
		SIZE_CHECK(func, glFuncMap);
		glAlphaFunc(glFuncMap[func], ref);
	}

	void cGDriver::ShadeModel(GLenum mode) {
		static GLenum shadeModelMap[2] = { GL_FLAT, GL_SMOOTH };
		SIZE_CHECK(mode, shadeModelMap);

		glShadeModel(shadeModelMap[mode]);
	}

	void cGDriver::Fog(uint32_t gdFogParamType, uint32_t gdFogParam) {
		static GLenum fogParamMap[] = { GL_EXP, GL_EXP2, GL_LINEAR, GL_FOG_COORD, GL_ZERO };
		SIZE_CHECK(gdFogParamType, fogParamTypeMap);
		SIZE_CHECK(gdFogParam, fogParamMap);

		glFogi(fogParamTypeMap[gdFogParamType], fogParamMap[gdFogParam]);
	}

	void cGDriver::Fog(uint32_t gdFogParamType, GLfloat const* params) {
		SIZE_CHECK(gdFogParamType, fogParamTypeMap);
		glFogfv(fogParamTypeMap[gdFogParamType], params);
	}

	void cGDriver::ColorMultiplier(float r, float g, float b) {
		colorMultiplierR = r;
		colorMultiplierG = g;
		colorMultiplierB = b;
		SetLightingParameters();
	}

	void cGDriver::AlphaMultiplier(float a) {
		colorMultiplierA = a;
		SetLightingParameters();
	}

	void cGDriver::SetLightingParameters() {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		EnableVertexColors(ambientMaterialEnabled, diffuseMaterialEnabled);

		GLfloat params[4] = { colorMultiplierR, colorMultiplierG, colorMultiplierB, colorMultiplierA };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, params);

		params[0] = 1.0f;
		params[1] = 0.0f;
		params[2] = 0.0f;
		params[3] = 1.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, params);

		params[0] = 0.0f;
		params[1] = 0.0f;
		params[2] = 0.0f;
		params[3] = colorMultiplierA;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, params);
	}

	void cGDriver::EnableVertexColors(bool ambient, bool diffuse) {
		ambientMaterialEnabled = ambient;
		diffuseMaterialEnabled = diffuse;

		if (!ambient && !diffuse) {
			glDisable(GL_COLOR_MATERIAL);
			return;
		}

		glEnable(GL_COLOR_MATERIAL);
		if (ambient && diffuse) {
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		}
		else if (diffuse) {
			glColorMaterial(GL_FRONT, GL_DIFFUSE);
		}
		else {
			glColorMaterial(GL_FRONT, GL_AMBIENT);
		}
	}

	void cGDriver::MatrixMode(GLenum mode) {
		const GLenum modeMap[] = { GL_MODELVIEW, GL_PROJECTION };
		SIZE_CHECK(mode, modeMap);

		glMatrixMode(modeMap[mode]);
	}

	void cGDriver::LoadMatrix(GLfloat const* m) {
		glLoadMatrixf(m);
	}

	void cGDriver::LoadIdentity(void) {
		glLoadIdentity();
	}

	void cGDriver::Enable(GLenum gdCap) {
		SIZE_CHECK(gdCap, capabilityMap);

		GLenum glCap = capabilityMap[gdCap];
		if (glCap != 0) {
			glEnable(glCap);
		}

		if (gdCap == kGDCapability_Texture2D) {
			textureStageData[activeTextureStage].toBeEnabled = true;
		}
	}

	void cGDriver::Disable(GLenum gdCap) {
		SIZE_CHECK(gdCap, capabilityMap);

		GLenum glCap = capabilityMap[gdCap];
		if (glCap != 0) {
			glDisable(glCap);
		}

		if (gdCap == kGDCapability_Texture2D) {
			textureStageData[activeTextureStage].toBeEnabled = false;
		}
	}

	bool cGDriver::IsEnabled(GLenum gdCap) {
		SIZE_CHECK_RETVAL(gdCap, capabilityMap, false);

		GLenum glCap = capabilityMap[gdCap];
		if (glCap != 0) {
			bool result = glIsEnabled(glCap) != 0;
			return result;
		}

		return false;
	}

	void cGDriver::GetBoolean(GLenum pname, bool* params) {
#ifndef NDEBUG
		if (pname != 0) {
			UNEXPECTED();
			return;
		}
#endif

		glGetBooleanv(GL_UNPACK_ROW_LENGTH, reinterpret_cast<GLboolean*>(params));
	}

	void cGDriver::GetInteger(GLenum pname, GLint* params) {
#ifndef NDEBUG
		if (pname != 0) {
			UNEXPECTED();
			return;
		}
#endif

		glGetIntegerv(GL_UNPACK_ROW_LENGTH, params);
	}

	void cGDriver::GetFloat(GLenum pname, GLfloat* params) {
#ifndef NDEBUG
		if (pname != 0) {
			UNEXPECTED();
			return;
		}
#endif

		glGetFloatv(GL_UNPACK_ROW_LENGTH, params);
	}

	void cGDriver::PolygonOffset(int32_t offset) {
		float fOffset = (float)offset;
		if (offset < 0) {
			fOffset += 4294967296.0f;
		}

		glPolygonOffset(0.0f, fOffset);
	}

	void cGDriver::BitBlt(
		int32_t destLeft,
		int32_t destTop,
		int32_t unknownWidth1,
		int32_t unknownHeight1,
		uint32_t gdTexFormat,
		uint32_t gdType,
		void const* unknownBuffer1,
		bool unknown5,
		void const* unknownBuffer2)
	{
		uint8_t const* unknownUintBuffer1 = reinterpret_cast<uint8_t const*>(unknownBuffer1);
		uint8_t const* unknownUintBuffer2 = reinterpret_cast<uint8_t const*>(unknownBuffer2);
		NOTIMPL();
		PRINT_VALUES(destLeft, destTop, unknownWidth1, unknownHeight1);
		PRINT_VALUES(gdTexFormat, gdType, unknownBuffer1, unknown5);
		PRINT_VALUES(unknownBuffer2, 0);

		SetLastError(DriverError::NOT_SUPPORTED);
	}

	void cGDriver::StretchBlt(
		int32_t destLeft,
		int32_t destTop,
		int32_t unknownWidth1,
		int32_t unknownHeight1,
		int32_t unknownWidth2,
		int32_t unknownHeight2,
		uint32_t gdTexFormat,
		uint32_t gdType,
		void const* unknownBuffer1,
		bool unknownBool,
		void const* unknownBuffer2)
	{
		uint8_t const* unknownUintBuffer1 = reinterpret_cast<uint8_t const*>(unknownBuffer1);
		uint8_t const* unknownUintBuffer2 = reinterpret_cast<uint8_t const*>(unknownBuffer2);
		NOTIMPL();
		PRINT_VALUES(destLeft, destTop, unknownWidth1, unknownHeight1);
		PRINT_VALUES(unknownWidth2, unknownHeight2, gdTexFormat, gdType);
		PRINT_VALUES(unknownBuffer1, unknownBool, unknownBuffer2);

		SetLastError(DriverError::NOT_SUPPORTED);
	}

	void cGDriver::BitBltAlpha(
		int32_t unknown0,
		int32_t unknown1,
		int32_t unknown2,
		int32_t unknown3,
		uint32_t gdTexFormat,
		uint32_t gdType,
		void const* unknownBuffer1,
		bool unknown5,
		void const* unknownBuffer2,
		uint32_t unknown7)
	{
		uint8_t const* unknownUintBuffer1 = reinterpret_cast<uint8_t const*>(unknownBuffer1);
		uint8_t const* unknownUintBuffer2 = reinterpret_cast<uint8_t const*>(unknownBuffer2);
		NOTIMPL();
		PRINT_VALUES(unknown0, unknown1, unknown2, unknown3);
		PRINT_VALUES(gdTexFormat, gdType, unknownBuffer1, unknown5);
		PRINT_VALUES(unknownBuffer2, unknown7);

		SetLastError(DriverError::NOT_SUPPORTED);
	}

	void cGDriver::StretchBltAlpha(
		int32_t destLeft,
		int32_t destTop,
		int32_t unknownWidth1,
		int32_t unknownHeight1,
		int32_t unknownWidth2,
		int32_t unknownHeight2,
		uint32_t gdTexFormat,
		uint32_t gdType,
		void const* unknownBuffer1,
		bool unknown7,
		void const* unknownBuffer2,
		uint32_t unknown9)
	{
		uint8_t const* unknownUintBuffer1 = reinterpret_cast<uint8_t const*>(unknownBuffer1);
		uint8_t const* unknownUintBuffer2 = reinterpret_cast<uint8_t const*>(unknownBuffer2);
		NOTIMPL();
		PRINT_VALUES(destLeft, destTop, unknownWidth1, unknownHeight1);
		PRINT_VALUES(unknownWidth2, unknownHeight2, gdTexFormat, gdType);
		PRINT_VALUES(unknownBuffer1, unknown7, unknownBuffer2, unknown9);


		SetLastError(DriverError::NOT_SUPPORTED);
	}

	void cGDriver::BitBltAlphaModulate(
		int32_t unknown0,
		int32_t unknown1,
		int32_t unknown2,
		uint32_t gdTexFormat,
		uint32_t gdType,
		void const* unknownBuffer1,
		bool unknown4,
		void const* unknownBuffer2,
		uint32_t unknown6)
	{
		uint8_t const* unknownUintBuffer1 = reinterpret_cast<uint8_t const*>(unknownBuffer1);
		uint8_t const* unknownUintBuffer2 = reinterpret_cast<uint8_t const*>(unknownBuffer2);
		NOTIMPL();
		PRINT_VALUES(unknown0, unknown1, unknown2, gdTexFormat);
		PRINT_VALUES(gdType, unknownBuffer1, unknown4, unknownBuffer2);
		PRINT_VALUES(unknown6, 0);

		SetLastError(DriverError::NOT_SUPPORTED);
	}

	void cGDriver::StretchBltAlphaModulate(
		int32_t destLeft,
		int32_t destTop,
		int32_t unknownWidth1,
		int32_t unknownHeight1,
		int32_t unknownWidth2,
		int32_t unknownHeight2,
		uint32_t gdTexFormat,
		uint32_t gdType,
		void const* unknownBuffer1,
		bool unknown7,
		void const* unknownBuffer2,
		uint32_t unknown9)
	{
		uint8_t const* unknownUintBuffer1 = reinterpret_cast<uint8_t const*>(unknownBuffer1);
		uint8_t const* unknownUintBuffer2 = reinterpret_cast<uint8_t const*>(unknownBuffer2);
		NOTIMPL();
		PRINT_VALUES(destLeft, destTop, unknownWidth1, unknownHeight1);
		PRINT_VALUES(unknownWidth2, unknownHeight2, gdTexFormat, gdType);
		PRINT_VALUES(unknownBuffer1, unknown7, unknownBuffer2, unknown9);

		SetLastError(DriverError::NOT_SUPPORTED);
	}

	bool cGDriver::Punt(uint32_t, void*) {
		return false;
	}

	
#ifndef NDEBUG
	static void GLAPIENTRY LogGLMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR) {
			// We know we're using the fixed function pipeline.
			// FUTURE: but what if we weren't?
			return;
		}

		static FILE* glMessageLog = NULL;
		if (glMessageLog == NULL) {
			glMessageLog = fopen("cGDriver.opengl.log", "w");
		}

		fprintf(glMessageLog, "[GLerr] [source: %x] [type: %x] [id: %x] [severity: %d] %s\n", source, type, id, severity, message);
		fflush(glMessageLog);
	}
#endif

	bool cGDriver::Init(void) {
		// if (SDL_Init) {
		// 	const char* errorDescription;
		// 	int errorCode = glfwGetError(&errorDescription);

		// 	char* error = new char[strlen(errorDescription) + 64];
		// 	sprintf(error, "Failed to initialize GLFW (error code: %d)\n%s", errorCode, errorDescription);
		// 	MessageBoxA(NULL, error, "SCGL failed to start", MB_ICONERROR);
		// 	delete[] error;

		// 	SetLastError(DriverError::CREATE_CONTEXT_FAIL);
		// 	return false;
		// }

		//glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		/*glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#ifdef NDEBUG
		glfwWindowHint(GLFW_NO_ERROR, GLFW_TRUE);
#endif
		//glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		// window = glfwCreateWindow(640, 480, "GDriverWindow--OpenGL", NULL, NULL);
		// glfwMakeContextCurrent(window);
		// glfwHideWindow(window);
		//InitGLSupport();
		window = SDL_CreateWindow("GDriverWindow--OpenGL",
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		deviceContext = (void*)SDL_GL_CreateContext(window);
		SDL_GL_MakeCurrent(window, (SDL_GLContext)deviceContext);
		//SDL_HideWindow(window);

        gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);

#ifndef NDEBUG
		if (glDebugMessageCallback != NULL) {
			glDebugMessageCallback(LogGLMessage, NULL);
			glEnable(GL_DEBUG_OUTPUT);
		}
#endif
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, reinterpret_cast<GLint*>(&maxTextureUnits));

		char const* vendor = reinterpret_cast<char const*>(glGetString(GL_VENDOR));
		char const* renderer = reinterpret_cast<char const*>(glGetString(GL_RENDERER));
		char const* version = reinterpret_cast<char const*>(glGetString(GL_VERSION));

		static const char unknownDriverName[] = "UnknownDriverName";
		static const char unknownCardVersion[] = "UnknownCardVersion";

		driverInfo.append(unknownDriverName, sizeof(unknownDriverName) - 1);
		driverInfo.append("\n", 1);
		driverInfo.append(version, strlen(version));
		driverInfo.append("\n", 1);
		driverInfo.append(renderer, strlen(renderer));
		driverInfo.append("\n", 1);
		driverInfo.append(unknownCardVersion, sizeof(unknownCardVersion) - 1);
		driverInfo.append("\n", 1);
		driverInfo.append(renderer, strlen(renderer));
		driverInfo.append("\n", 1);

        //printf(driverInfo.c_str());

		InitializeVideoModeVector();
		return true;
	}

	bool cGDriver::Shutdown(void) {
		if (window != NULL) {
			SDL_DestroyWindow(window);
			window = NULL;
			SDL_GL_DeleteContext((SDL_GLContext)deviceContext);
			deviceContext = NULL;
		}

		return true;
	}

	int32_t cGDriver::InitializeVideoModeVector(void) {
		videoModeCount = SDL_GetNumDisplayModes(0);
		// const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &videoModeCount);
		if (videoModeCount < 1) {
            printf("ERROR: No display modes! %s\n", SDL_GetError());
			return 0;
		}
		SDL_DisplayMode* modes = (SDL_DisplayMode*)malloc(sizeof(SDL_DisplayMode) * videoModeCount);
		for (int i = 0; i < videoModeCount; i++) {
			if (SDL_GetDisplayMode(0, i, &modes[i]) < 0) {
              printf("Error getting display mode at index %d!\n%s\n",i,
                     SDL_GetError());
              exit(-1);
            }
		}

		sGDMode tempMode{};

		// If not set, SC4 throws the "Could not initialize the hardware driver" error and switches to software mode.
		tempMode.isInitialized = true;

		// TODO: do we actually need to check all these extensions or are they all part of the OpenGL 3.0 core profile?
		tempMode.supportsStencilBuffer = true;
		tempMode.supportsMultitexture = true;
		tempMode.supportsTextureEnvCombine = true;
		tempMode.supportsNvTextureEnvCombine4 = true;
		tempMode.supportsFogCoord = true;
		tempMode.supportsDxtTextures = true;
		tempMode.textureStageCount = maxTextureUnits;

		// TODO: what are these flags for and why does the game's OpenGL driver set them?
		tempMode.__unknown2 = true;
		tempMode.__unknown5[0] = false;
		tempMode.__unknown5[1] = false;
		tempMode.__unknown5[2] = false;

		int i = 0;
		while (i < videoModeCount) {
			int depth = SDL_BITSPERPIXEL(modes[i].format);

			if (depth > 16) {
				tempMode.alphaColorMask = 0xff000000;
				tempMode.redColorMask = 0x00ff0000;
				tempMode.greenColorMask = 0x0000ff00;
				tempMode.blueColorMask = 0x000000ff;
			}
			else {
				tempMode.alphaColorMask = 0x1;
				tempMode.redColorMask = 0xf800;
				tempMode.greenColorMask = 0x7c0;
				tempMode.blueColorMask = 0x3e;
			}

			tempMode.index = i++;
			tempMode.width = modes[i].w;
			tempMode.height = modes[i].h;
			tempMode.depth = depth;

			/*tempMode.isFullscreen = true;

			videoModes.push_back(tempMode);

			tempMode.index = i++;*/
			tempMode.isFullscreen = false;

			videoModes.push_back(tempMode);

            PRINT_VALUES(i, videoModes[i - 1].width, videoModes[i - 1].height,
                         videoModes[i - 1].depth);
		}
		free(modes);


		return videoModeCount;
	}
	
	bool cGDriver::BufferRegionEnabled(void) {
		return true;
	}

	uint32_t cGDriver::NewBufferRegion(int32_t gdBufferRegionType) {
// 		static GLenum regionTypeMap[] = { GL_KTX_BACK_REGION, GL_KTX_Z_REGION, GL_KTX_STENCIL_REGION };

// #ifndef NDEBUG
// 		if (gdBufferRegionType >= sizeof(regionTypeMap) / sizeof(regionTypeMap[0])) {
// 			UNEXPECTED();
// 			return 0;
// 		}
// #endif

// 		GLuint result = glNewBufferRegion(regionTypeMap[gdBufferRegionType]);
// 		if (result != 0) {
// 			bufferRegionFlags |= 1 << (result - 1);
// 		}

// 		return result;

		// we don't have GLEW
		return 0;
	}

	bool cGDriver::DeleteBufferRegion(int32_t region) {
		/* glDeleteBufferRegion(region);
		bufferRegionFlags &= ~(1 << (region - 1));
		return true; */
		return true;
	}

	bool cGDriver::ReadBufferRegion(uint32_t region, GLint x, GLint y, GLsizei width, GLsizei height, int32_t unused0, int32_t unused1) {
		/* glReadBufferRegion(region, x, y, width, height);
		return true; */
		return true;
	}

	bool cGDriver::DrawBufferRegion(uint32_t region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest) {
		/* glDrawBufferRegion(region, x, y, width, height, xDest, yDest);
		return true; */
        return true;
	}

	bool cGDriver::IsBufferRegion(uint32_t region) {
		return (bufferRegionFlags & (1 << (region - 1))) != 0;
	}

	bool cGDriver::CanDoPartialRegionWrites(void) {
		return /* true */true;
	}

	bool cGDriver::CanDoOffsetReads(void) {
		return /* true */true;
	}

	bool cGDriver::DeleteAllBufferRegions(void) {
		for (size_t i = 0; i < sizeof(bufferRegionFlags) * 8; i++) {
			if (IsBufferRegion(i)) {
				DeleteBufferRegion(i);
			}
		}

		bufferRegionFlags = 0;
		return true;
	}

	
	void cGDriver::EnableLighting(bool flag) {
		flag ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	}

	void cGDriver::EnableLight(uint32_t light, bool flag) {
		flag ? glEnable(GL_LIGHT0 + light) : glDisable(GL_LIGHT0 + light);
	}

	void cGDriver::LightModelAmbient(float r, float g, float b, float a) {
		GLfloat params[] = { r, g, b, a };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, params);
	}

	void cGDriver::LightColor(uint32_t lightIndex, uint32_t gdParam, float const* color) {
		static GLenum colorParamMap[] = { GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR };

#ifndef NDEBUG
		if (gdParam >= sizeof(colorParamMap) / sizeof(colorParamMap[0])) {
			UNEXPECTED();
			return;
		}
#endif

		GLenum param = colorParamMap[gdParam];
		glLightfv(GL_LIGHT0 + lightIndex, param, color);
	}

	void cGDriver::LightColor(uint32_t lightIndex, float const* ambient, float const* diffuse, float const* specular) {
		GLenum light = GL_LIGHT0 + lightIndex;
		if (ambient) {
			glLightfv(light, GL_AMBIENT, ambient);
		}

		if (diffuse) {
			glLightfv(light, GL_DIFFUSE, diffuse);
		}

		if (specular) {
			glLightfv(light, GL_SPECULAR, specular);
		}
	}

	void cGDriver::LightPosition(uint32_t lightIndex, float const* position) {
		glLightfv(GL_LIGHT0 + lightIndex, GL_POSITION, position);
	}

	void cGDriver::LightDirection(uint32_t lightIndex, float const* direction) {
		NOTIMPL();
	}

	void cGDriver::MaterialColor(uint32_t gdParam, float const* color) {
		static GLenum materialParamMap[] = { GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR, GL_EMISSION, GL_SHININESS };

#ifndef NDEBUG
		if (gdParam >= sizeof(materialParamMap) / sizeof(materialParamMap[0])) {
			UNEXPECTED();
			return;
		}
#endif

		GLenum param = materialParamMap[gdParam];
		glMaterialfv(GL_FRONT, param, color);
	}

	void cGDriver::MaterialColor(
		float const* ambient,
		float const* diffuse,
		float const* specular,
		float const* emission,
		float shininess)
	{
		if (ambient) {
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
		}

		if (diffuse) {
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		}

		if (specular) {
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		}

		if (emission) {
			glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		}

		if (shininess >= 0.0f) {
			glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
		}
	}

	
	static GLenum texEnvParamMap[2] = { GL_TEXTURE_ENV_MODE, GL_TEXTURE_ENV_COLOR };
	static GLenum internalFormatMap[8] = {
		GL_RGB5, GL_RGB8, GL_RGBA4, GL_RGB5_A1,
		GL_RGBA8, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
		GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
	};

	static GLenum formatMap[11] = {
		GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_ALPHA, GL_LUMINANCE, GL_LUMINANCE_ALPHA,
		GL_COMPRESSED_RGB_S3TC_DXT1_EXT, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
		GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
	};

	extern GLenum typeMap[16];

	void cGDriver::ApplyTextureStages() {
		for (uint32_t i = 0; i < maxTextureUnits; i++) {
			TextureStageData& texStage = textureStageData[i];
			if (!texStage.toBeEnabled) {
				if (texStage.currentlyEnabled) {
					if (videoModes[currentVideoMode].supportsMultitexture) {
						glClientActiveTexture(GL_TEXTURE0 + i);
					}

					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					texStage.currentlyEnabled = false;
					texStage.textureHandle = NULL;
				}
			}
			else {
				if (videoModes[currentVideoMode].supportsMultitexture) {
					glClientActiveTexture(GL_TEXTURE0 + i);
				}

				if (!texStage.currentlyEnabled) {
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					texStage.currentlyEnabled = true;
				}

				void const* textureHandle = reinterpret_cast<uint8_t const*>(interleavedPointer) + VertexFormatElementOffset(interleavedFormat, 7, texStage.coordSrc);
				if (texStage.textureHandle != textureHandle) {
					glTexCoordPointer(2, GL_FLOAT, interleavedStride, textureHandle);
					texStage.textureHandle = textureHandle;
				}
			}
		}
	}

	void cGDriver::GenTextures(GLsizei n, GLuint* textures) {
		glGenTextures(n, textures);
	}

	void cGDriver::DeleteTextures(GLsizei n, GLuint const* textures) {
		glDeleteTextures(n, textures);
	}

	bool cGDriver::IsTexture(GLuint texture) {
		bool result = glIsTexture(texture) != 0;
		return result;
	}

	void cGDriver::PrioritizeTextures(GLsizei n, GLuint const* textures, GLclampf const* priorities) {
		glPrioritizeTextures(n, textures, priorities);
	}

	bool cGDriver::AreTexturesResident(GLsizei n, GLuint const* textures, bool* residences) {
		bool result = glAreTexturesResident(n, textures, reinterpret_cast<GLboolean*>(residences)) != 0;
		return result;
	}

	void cGDriver::BindTexture(GLenum target, GLuint texture) {
#ifndef NDEBUG
		if (target > 0) {
			UNEXPECTED();
			return;
		}
#endif

		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void cGDriver::TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, void const* pixels) {
#ifndef NDEBUG
		if (target > 0) {
			UNEXPECTED();
			return;
		}
#endif

		NOTIMPL();
		glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, border, format, type, pixels);
	}

	void cGDriver::PixelStore(GLenum pname, GLint param) {
#ifndef NDEBUG
		if (pname > 0) {
			UNEXPECTED();
			return;
		}
#endif

		glPixelStorei(GL_UNPACK_ROW_LENGTH, param);
	}

	void cGDriver::TexEnv(GLenum target, GLenum pname, GLint gdParam) {
		GLint paramMap[] = { GL_REPLACE, GL_MODULATE, GL_DECAL, GL_BLEND, GL_COMBINE, GL_COMBINE4_NV };
#ifndef NDEBUG
		if (pname >= sizeof(texEnvParamMap) / sizeof(texEnvParamMap[0]) || gdParam >= sizeof(paramMap) / sizeof(paramMap[0])) {
			UNEXPECTED();
			return;
		}
#endif

		glTexEnvi(GL_TEXTURE_ENV, texEnvParamMap[pname], paramMap[gdParam]);
	}

	void cGDriver::TexEnv(GLenum target, GLenum pname, GLfloat const* params) {
#ifndef NDEBUG
		if (pname >= sizeof(texEnvParamMap) / sizeof(texEnvParamMap[0])) {
			UNEXPECTED();
			return;
		}
#endif

		glTexEnvfv(GL_TEXTURE_ENV, texEnvParamMap[pname], params);
	}

	void cGDriver::TexParameter(GLenum target, GLenum pname, GLint param) {
		static GLenum texParamNameMap[] = { GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T };
		static GLenum texParamMap[] = { GL_NEAREST, GL_LINEAR, GL_CLAMP, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR };

#ifndef NDEBUG
		if (pname >= sizeof(texParamNameMap) / sizeof(texParamNameMap[0]) || param >= sizeof(texParamMap) / sizeof(texParamMap[0])) {
			UNEXPECTED();
			return;
		}
#endif

		glTexParameteri(GL_TEXTURE_2D, texParamNameMap[pname], texParamMap[param]);
	}

	void cGDriver::TexStage(GLenum texUnit) {
		if (videoModes[currentVideoMode].supportsMultitexture || texUnit != 0) {
			if (texUnit < maxTextureUnits) {
				activeTextureStage = texUnit;
				glClientActiveTexture(GL_TEXTURE0 + texUnit);
				return;
			}

			SetLastError(DriverError::INVALID_VALUE);
		}
	}

	void cGDriver::TexStageCoord(uint32_t gdTexCoordSource) {
		static float sCoord[] = { 1.0f, 0.0f, 0.0f, 0.0f };
		static float tCoord[] = { 0.0f, 1.0f, 0.0f, 0.0f };

		textureStageData[activeTextureStage].coordSrc = gdTexCoordSource;

		//NOTIMPL();
		if ((gdTexCoordSource & 0xfffffff8) == 0x10) {
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glTexGenfv(GL_S, GL_EYE_PLANE, sCoord);
			glTexGenfv(GL_T, GL_EYE_PLANE, tCoord);
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		}
		else {
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
		}
	}

	void cGDriver::TexStageMatrix(GLfloat const* matrix, uint32_t unknown0, uint32_t unknown1, uint32_t gdTexMatFlags) {
		glMatrixMode(GL_TEXTURE);
		if (matrix == NULL) {
			glLoadIdentity();
			return;
		}

		if ((gdTexMatFlags & 3) == 1 && unknown0 == 4 && unknown1 == 2) {
			GLfloat replacementMatrix[16];
			GLfloat* replacementPtr = replacementMatrix;
			for (int i = 0; i < 16; i++) {
				*replacementPtr = *(matrix++);
			}

			replacementMatrix[2] = 0.0f;
			replacementMatrix[6] = 0.0f;
			replacementMatrix[10] = 1.0f;
			replacementMatrix[14] = 0.0f;

			replacementMatrix[3] = 0.0f;
			replacementMatrix[7] = 0.0f;
			replacementMatrix[11] = 0.0f;
			replacementMatrix[15] = 1.0f;

			glLoadMatrixf(replacementMatrix);
			return;
		}

		if ((gdTexMatFlags & 1) == 0 || (unknown0 > 3 && (unknown1 > 3 || (gdTexMatFlags & 2) == 0))) {
			glLoadMatrixf(matrix);
		}
	}

	void cGDriver::TexStageCombine(eGDTextureStageCombineParamType gdParamType, eGDTextureStageCombineModeParam gdParam) {
		static GLenum pnameMap[] = { GL_COMBINE_RGB, GL_COMBINE_ALPHA };
		static GLint paramMap[] = { GL_REPLACE, GL_MODULATE, GL_ADD, GL_ADD_SIGNED, GL_INTERPOLATE, GL_DOT3_RGB };

#ifndef NDEBUG
		if ((int)gdParamType >= sizeof(pnameMap) / sizeof(pnameMap[0]) || (int)gdParam >= sizeof(paramMap) / sizeof(paramMap[0])) {
			UNEXPECTED();
			return;
		}
#endif

		if (!videoModes[currentVideoMode].supportsTextureEnvCombine) {
			SetLastError(DriverError::INVALID_VALUE);
			return;
		}

		glTexEnvi(GL_TEXTURE_ENV, pnameMap[(int)gdParamType], paramMap[(int)gdParam]);
	}

	void cGDriver::TexStageCombine(eGDTextureStageCombineSourceParamType gdParamType, eGDTextureStageCombineSourceParam gdParam) {
		static GLenum pnameMap[] = { GL_SRC0_RGB, GL_SRC1_RGB, GL_SRC2_RGB, GL_SOURCE3_RGB_NV, GL_SRC0_ALPHA, GL_SRC1_ALPHA, GL_SRC2_ALPHA, GL_SOURCE3_ALPHA_NV };
		static GLint paramMap[] = { GL_TEXTURE, GL_PREVIOUS, GL_CONSTANT, GL_PRIMARY_COLOR };

#ifndef NDEBUG
		if ((int)gdParamType >= sizeof(pnameMap) / sizeof(pnameMap[0]) || (int)gdParam >= sizeof(paramMap) / sizeof(paramMap[0])) {
			UNEXPECTED();
			return;
		}
#endif

		if (!videoModes[currentVideoMode].supportsTextureEnvCombine) {
			SetLastError(DriverError::INVALID_VALUE);
			return;
		}

		glTexEnvi(GL_TEXTURE_ENV, pnameMap[(int)gdParamType], paramMap[(int)gdParam]);
	}

	void cGDriver::TexStageCombine(eGDTextureStageCombineOperandType gdParamType, eGDTextureStageCombineOperand gdBlend) {
		static GLenum pnameMap[] = { GL_OPERAND0_RGB, GL_OPERAND1_RGB, GL_OPERAND2_RGB, GL_OPERAND3_RGB_NV, GL_OPERAND0_ALPHA, GL_OPERAND1_ALPHA, GL_OPERAND2_ALPHA, GL_OPERAND3_ALPHA_NV };
		static GLint paramMap[] = {
			GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
			GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA_SATURATE
		};

#ifndef NDEBUG
		if ((int)gdParamType >= sizeof(pnameMap) / sizeof(pnameMap[0]) || (int)gdBlend >= sizeof(paramMap) / sizeof(paramMap[0])) {
			UNEXPECTED();
			return;
		}
#endif

		if (!videoModes[currentVideoMode].supportsTextureEnvCombine) {
			SetLastError(DriverError::INVALID_VALUE);
			return;
		}

		glTexEnvi(GL_TEXTURE_ENV, pnameMap[(int)gdParamType], paramMap[(int)gdBlend]);
	}

	void cGDriver::TexStageCombine(eGDTextureStageCombineScaleParamType gdPname, eGDTextureStageCombineScaleParam gdParam) {
		static GLenum pnameMap[] = { GL_RGB_SCALE, GL_ALPHA_SCALE };
		static GLfloat paramMap[] = { 1.0f, 2.0f, 4.0f };

#ifndef NDEBUG
		if ((int)gdPname >= sizeof(pnameMap) / sizeof(pnameMap[0]) || (int)gdParam >= sizeof(paramMap) / sizeof(paramMap[0])) {
			UNEXPECTED();
			return;
		}
#endif

		if (!videoModes[currentVideoMode].supportsTextureEnvCombine) {
			SetLastError(DriverError::INVALID_VALUE);
			return;
		}

		glTexEnvfv(GL_TEXTURE_ENV, pnameMap[(int)gdPname], &paramMap[(int)gdParam]);
	}

	void cGDriver::SetTexture(GLuint textureId, GLenum texUnit) {
		glActiveTexture(GL_TEXTURE0 + texUnit);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	intptr_t cGDriver::GetTexture(GLenum texUnit) {
		glActiveTexture(GL_TEXTURE0 + texUnit);

		int activeTexture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &activeTexture);
		return activeTexture;
	}

	intptr_t cGDriver::CreateTexture(uint32_t texformat, uint32_t width, uint32_t height, uint32_t levels, uint32_t texhints) {
		GLuint textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		int numLevels = 1;
		if (levels != 0) {
			numLevels = levels;
		}

		GLint internalFormat = internalFormatMap[texformat];
		for (int i = 0; i < numLevels; i++) {
			glTexImage2D(GL_TEXTURE_2D, i, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			width = (width < 2) ? 1 : (width >> 1);
			height = (height < 2) ? 1 : (height >> 1);
		}

		return textureId;
	}

    void cGDriver::LoadTextureLevel(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, uint32_t gdTexFormat, uint32_t gdType, uint32_t rowLength, void const* pixels) {
		GLenum glFormat = formatMap[gdTexFormat];
		GLenum glType = typeMap[gdType];

		glGetError();
		glBindTexture(GL_TEXTURE_2D, texture);
		glGetError();

		GLint texParamWidth, texParamHeight, internalFormat;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH, &texParamWidth);

		if (glGetError() == GL_NO_ERROR && texParamWidth != 0) {
			glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT, &texParamHeight);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);

			if (glFormat >= GL_COMPRESSED_RGB_S3TC_DXT1_EXT && glFormat <= GL_COMPRESSED_RGBA_S3TC_DXT5_EXT) {
				GLsizei size = ((width + 3) >> 2) * ((height + 3) >> 2) * (8 + (glFormat >= GL_COMPRESSED_RGBA_S3TC_DXT3_EXT ? 8 : 0));
				if (xoffset == 0 && yoffset == 0 && width == texParamWidth && height == texParamHeight) {
					glCompressedTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0, size, pixels);
				}
				else {
					glCompressedTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width, height, glFormat, size, pixels);
				}

				return;
			}

			glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);

			if (xoffset == 0 && yoffset == 0 && width == texParamWidth && height == texParamHeight) {
				glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0, glFormat, glType, pixels);
			}
			else {
				glTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width, height, glFormat, glType, pixels);
			}
		}
	}

	void cGDriver::SetCombiner(cGDCombiner const& combiner, uint32_t texUnit) {
		static eGDTextureStageCombineOperand operandMap[] = {
			eGDTextureStageCombineOperand::SrcColor,
			eGDTextureStageCombineOperand::OneMinusSrcColor,
			eGDTextureStageCombineOperand::SrcAlpha,
			eGDTextureStageCombineOperand::OneMinusSrcAlpha,
			eGDTextureStageCombineOperand::SrcAlpha,
			eGDTextureStageCombineOperand::OneMinusSrcAlpha,
			eGDTextureStageCombineOperand::SrcAlpha,
			eGDTextureStageCombineOperand::OneMinusSrcAlpha,
		};

		TexStage(texUnit);
		TexEnv(0, kGDTextureEnvParamType_Mode, kGDTextureEnvParam_Combine);

		TexStageCombine(eGDTextureStageCombineParamType::RGB, (eGDTextureStageCombineModeParam)combiner.RGBCombineMode);
		TexStageCombine(eGDTextureStageCombineScaleParamType::RGB, (eGDTextureStageCombineScaleParam)combiner.RGBScale);
		TexStageCombine(eGDTextureStageCombineOperandType::Operand0RGB, (eGDTextureStageCombineOperand)operandMap[combiner.RGBParams[0].OperandType]);
		TexStageCombine(eGDTextureStageCombineSourceParamType::Src0RGB, (eGDTextureStageCombineSourceParam)combiner.RGBParams[0].SourceType);
		TexStageCombine(eGDTextureStageCombineOperandType::Operand1RGB, (eGDTextureStageCombineOperand)operandMap[combiner.RGBParams[1].OperandType]);
		TexStageCombine(eGDTextureStageCombineSourceParamType::Src1RGB, (eGDTextureStageCombineSourceParam)combiner.RGBParams[1].SourceType);
		TexStageCombine(eGDTextureStageCombineOperandType::Operand2RGB, (eGDTextureStageCombineOperand)operandMap[combiner.RGBParams[2].OperandType]);
		TexStageCombine(eGDTextureStageCombineSourceParamType::Src2RGB, (eGDTextureStageCombineSourceParam)combiner.RGBParams[2].SourceType);

		TexStageCombine(eGDTextureStageCombineScaleParamType::Alpha, (eGDTextureStageCombineScaleParam)combiner.AlphaScale);
		TexStageCombine(eGDTextureStageCombineParamType::Alpha, (eGDTextureStageCombineModeParam)combiner.AlphaCombineMode);
		TexStageCombine(eGDTextureStageCombineOperandType::Operand0Alpha, (eGDTextureStageCombineOperand)operandMap[combiner.AlphaParams[0].OperandType]);
		TexStageCombine(eGDTextureStageCombineSourceParamType::Src0Alpha, (eGDTextureStageCombineSourceParam)combiner.AlphaParams[0].SourceType);
		TexStageCombine(eGDTextureStageCombineOperandType::Operand1Alpha, (eGDTextureStageCombineOperand)operandMap[combiner.AlphaParams[1].OperandType]);
		TexStageCombine(eGDTextureStageCombineSourceParamType::Src1Alpha, (eGDTextureStageCombineSourceParam)combiner.AlphaParams[1].SourceType);
		TexStageCombine(eGDTextureStageCombineOperandType::Operand2Alpha, (eGDTextureStageCombineOperand)operandMap[combiner.AlphaParams[2].OperandType]);
		TexStageCombine(eGDTextureStageCombineSourceParamType::Src2Alpha, (eGDTextureStageCombineSourceParam)combiner.AlphaParams[2].SourceType);
	}

	
	uint32_t cGDriver::CountVideoModes(void) const {
		return videoModeCount;
	}

	void cGDriver::GetVideoModeInfo(uint32_t dwIndex, sGDMode *gdMode) {
      // TODO: remove this
      // hack: modify cGZGraphicSystem::mDesiredStartupDisplayMetrics
      // (which should be private but in this case we have to make it not
      // so) to reflect the best display mode.
      if (cGZGraphicSystem::mDesiredStartupDisplayMetrics.height < 480) {
        cGZGraphicSystem::mDesiredStartupDisplayMetrics.width = 1280;
        cGZGraphicSystem::mDesiredStartupDisplayMetrics.height = 720;
        cGZGraphicSystem::mDesiredStartupDisplayMetrics.bpp = 32;
      }

      PRINT_VALUES(dwIndex, gdMode);
		if (dwIndex == -1 || dwIndex >= (uint32_t)videoModeCount) {
			SetLastError(DriverError::OUT_OF_RANGE);
            printf("[ERROR] cGDriver::GetVideoModeInfo FAILED!\n");
			return;
		}

		sGDMode tmp = videoModes[dwIndex];
		*gdMode = tmp;
	}

	void cGDriver::GetVideoModeInfo(sGDMode *gdMode) {
		return GetVideoModeInfo(currentVideoMode, gdMode);
	}

	void cGDriver::SetVideoMode(int32_t newModeIndex, void* hwndProc, bool showWindow, bool) {
		if (newModeIndex == -1) {
			SDL_HideWindow(window);

			currentVideoMode = -1;
			windowWidth = 0;
			windowHeight = 0;

			SetLastError(DriverError::OK);
		}
		else if (newModeIndex < videoModeCount) {
			sGDMode const& newMode = videoModes[newModeIndex];
			//GLFWmonitor* monitor = NULL;

			currentVideoMode = newModeIndex;
			windowWidth = newMode.width;
			windowHeight = newMode.height;

			// if (hwndProc == NULL) {
			// 	hwndProc = DefWindowProcA;
			// }

			//HWND hwnd = glfwGetWin32Window(window);
			//SetWindowLong(hwnd, GWL_WNDPROC, reinterpret_cast<LONG>(hwndProc));

			// glfwSetWindowSize(window, windowWidth, windowHeight);
			// glfwShowWindow(window);

			// if (newMode.isFullscreen) {
			// 	monitor = glfwGetPrimaryMonitor();
			// 	glfwSetWindowMonitor(window, monitor, 0, 0, windowWidth, windowHeight, GLFW_DONT_CARE);
			// }
			SDL_SetWindowFullscreen(window, 0);

			SDL_SetWindowSize(window, windowWidth, windowHeight);
			SDL_ShowWindow(window);

			if (newMode.isFullscreen) {
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			SetViewport();
			SetLastError(DriverError::OK);
		}
		else {
			SetLastError(DriverError::OUT_OF_RANGE);
		}
	}

	bool cGDriver::IsDeviceReady(void) {
		return true;
	}

	void cGDriver::Flush(void) {
		glFlush();
		if (window != NULL) {
			SDL_GL_SwapWindow(window);
		}
	}

	void cGDriver::SetViewport(void) {
		glViewport(0, 0, windowWidth, windowHeight);
		glDisable(GL_SCISSOR_TEST);

		viewportX = viewportY = 0;
		viewportWidth = windowWidth;
		viewportHeight = windowHeight;
	}

	void cGDriver::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
		glViewport(x, y, width, height);
		glScissor(x, y, width, height);
		glEnable(GL_SCISSOR_TEST);

		viewportX = x;
		viewportY = y;
		viewportWidth = width;
		viewportHeight = height;
	}

	void cGDriver::GetViewport(int32_t dimensions[4]) {
		dimensions[0] = viewportX;
		dimensions[1] = viewportY;
		dimensions[2] = viewportX + viewportWidth;
		dimensions[3] = viewportY + viewportHeight;
	}
}