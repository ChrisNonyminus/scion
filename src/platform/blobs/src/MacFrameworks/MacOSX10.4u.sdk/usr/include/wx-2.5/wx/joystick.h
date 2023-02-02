#ifndef _WX_JOYSTICK_H_BASE_
#define _WX_JOYSTICK_H_BASE_

#if wxUSE_JOYSTICK

#if defined(__WXMSW__)
#include "wx/msw/joystick.h"
#elif defined(__WXMOTIF__)
#include "wx/unix/joystick.h"
#elif defined(__WXGTK__)
#include "wx/unix/joystick.h"
#elif defined(__WXX11__)
#include "wx/unix/joystick.h"
#elif defined(__WXMAC__)
#include "wx/mac/joystick.h"
#elif defined(__WXPM__)
#include "wx/os2/joystick.h"
#endif

#endif // wxUSE_JOYSTICK

#endif
    // _WX_JOYSTICK_H_BASE_
