//
// Created by chris on 1/17/2023.
//

#ifndef SCION_GZWIN_H
#define SCION_GZWIN_H

#include "cIGZUnknown.h"
#include "cRZSystemService.h"
#include "cRZCriticalSection.h"
#include "cRZKeyboard.h"
#include "cRZAutoRefCount.h"
#include "cIGZMessageTarget2.h"


#include <list>
#include <unordered_set>
#include <map>

#ifdef USING_SDL2
#include "include/SDL.h"
#endif

struct cIGZWin;

struct cIGZMessageFilter : public cIGZUnknown {};

struct cIGZWinMgr : public cIGZUnknown {
};
static const GZIID GZIID_cIGZWinMgr = 1444;
static const GZGUID RZSRVID_cGZWinMgr = 0xA417445E;

struct cGZWinMgrBase : public cIGZWinMgr, public cRZSystemService {
    struct MessageTargetInfo {};
    std::list<MessageTargetInfo> m_MTIList;
    cRZCriticalSection m_mCriticalSection;
    int field_30{0}, field_34{0}, field_38{0}, field_3C{0}, field_40{0},
            field_44{0};
    std::unordered_set<cIGZWin *> m_usIGZWins;
    int field_5C{0};
    cRZKeyboard m_Keyboard;
    int field_16C{0}, field_170{0}, field_174{0}, field_178{0}, field_17C{0},
            field_180{0}, field_184{0}, field_188{1};
    std::list<cIGZWin *> m_WinList1;
    int field_194{0}, field_198{0};
    std::list<cIGZWin *> m_WinList2;
    int field_1A4{1};
    std::map<uint32_t, cIGZWin *> m_WinMap;
    std::list<cRZAutoRefCount<cIGZMessageFilter>> m_MsgFilters;
    bool field_1C8{true}, field_1C9{false}, field_1CA{true}, field_1CB{true},
            field_1CC{true}, field_1CD{false}, field_1CE{true};
    cGZWinMgrBase();

    bool QueryInterface(GZIID iid, void **outPtr) override;

    uint32_t AddRef(void) override;

    uint32_t Release(void) override;
};

struct cIGZWinCtrlMgr : public cIGZUnknown {
};
static const GZIID GZIID_cIGZWinCtrlMgr = 0x22C2EB1F;
static const GZGUID RZSRVID_cGZWinCtrlMgr = 0xC2C2EB0F;

struct cGZWinCtrlMgr : public cIGZWinCtrlMgr, public cRZSystemService {
    int field_20{0}, field_24{0}, field_28{0};
};

#ifdef USING_SDL2
struct cGZWinMgrSDL2 : public cGZWinMgrBase {
    cGZWinMgrSDL2();

    SDL_Window *m_Window;
    SDL_Renderer *m_Renderer;
    SDL_Texture *m_Texture;
#ifdef USING_OPENGL
    SDL_GLContext m_GLContext;
#endif
};
#endif

#ifdef _WIN32
struct cGZWinMgrW32 : public cGZWinMgrBase {
    struct cCanvasMessageTarget : public cIGZMessageTarget2 {
        ~cCanvasMessageTarget();
        uint32_t AddRef() override;
        uint32_t Release() override;
        bool QueryInterface(uint32_t uiClsId, void **ppvObj) override;
    };
    struct cMouseMessageTarget : public cIGZMessageTarget2 {
        ~cMouseMessageTarget();
        uint32_t AddRef() override;
        uint32_t Release() override;
        bool QueryInterface(uint32_t uiClsId, void **ppvObj) override;
    };
    struct cKeyMessageTarget : public cIGZMessageTarget2 {
        ~cKeyMessageTarget();
        uint32_t AddRef() override;
        uint32_t Release() override;
        bool QueryInterface(uint32_t uiClsId, void **ppvObj) override;
    };
};
#endif

#endif //SCION_GZWIN_H
