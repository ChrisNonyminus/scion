#include "cIGZUnknown.h"
#include "cIGZFramework.h"
#include "cRZCOMDllDirector.h"
#include "cGDriver.h"

cIGZUnknown* GZCOM_CreatecSGLDriverDX7() {
	return static_cast<cIGZGDriver*>(((new nSCGL::cGDriver)));
}