#include "cGDriver.h"

namespace nSCGL
{
	bool cGDriver::QueryInterface(uint32_t riid, void** ppvObj) {
		switch (riid)
		{
		case GZIID_cIGZUnknown:
		case GZIID_cIGZGDriver:
			*ppvObj = static_cast<cIGZGDriver*>(this);
			break;

		case GZIID_cIGZGBufferRegionExtension:
			/* if (glNewBufferRegion == NULL || !glBufferRegionEnabled()) {
				// Intel does not support GL_KTX_buffer_regions, so let's not even expose the interface.
				// FUTURE: there's a better supported extension, WGL_ARB_buffer_region, that does the
				// same thing. It would be nice to use that instead.
				return false;
			}

			*ppvObj = static_cast<cIGZGBufferRegionExtension*>(this); */
			return false;
			break;

		case GZIID_cIGZGDriverLightingExtension:
			*ppvObj = static_cast<cIGZGDriverLightingExtension*>(this);
			break;

		case GZIID_cIGZGDriverVertexBufferExtension:
			*ppvObj = static_cast<cIGZGDriverVertexBufferExtension*>(this);
			break;

		case GZIID_cIGZGSnapshotExtension:
			*ppvObj = static_cast<cIGZGSnapshotExtension*>(this);
			break;

		default:
			char buf[512];
			sprintf(buf, "Unknown interface ID in GDriver: %x", riid);
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SCGL", buf, window);
			return false;
		}

		AddRef();
		return true;
	}

	uint32_t cGDriver::AddRef(void) {
		return cRZRefCount::AddRef();
	}

	uint32_t cGDriver::Release(void) {
		return cRZRefCount::Release();
	}

	bool cGDriver::FinalRelease(void) {
		NOTIMPL();
		return false;
	}
}