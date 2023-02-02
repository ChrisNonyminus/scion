//
// Created by chris on 1/17/2023.
//
#include "GZWin.h"

#ifdef USING_SDL2

#include "include/SDL.h"

#endif

cGZWinMgrBase::cGZWinMgrBase()
        : cRZSystemService(RZSRVID_cGZWinMgr, 1250000) {}

bool cGZWinMgrBase::QueryInterface(GZIID iid, void **outPtr) {
    if (iid == GZIID_cIGZWinMgr) {
        *outPtr = static_cast<cIGZWinMgr *>(this);
        return true;
    }
    return cRZSystemService::QueryInterface(iid, outPtr);
}

uint32_t cGZWinMgrBase::AddRef(void) {
    return cRZSystemService::AddRef();
}

uint32_t cGZWinMgrBase::Release(void) {
    return cRZSystemService::Release();
}

#ifdef USING_SDL2

cGZWinMgrSDL2::cGZWinMgrSDL2() {
#ifdef USING_OPENGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
    int result = SDL_InitSubSystem(SDL_INIT_VIDEO);
    if (result != 0) {
        printf("SDL_InitSubSystem failed: %s\n", SDL_GetError());
        return;
    }
    int winFlags = 0;
#if defined(USING_OPENGL)
    winFlags |= SDL_WINDOW_OPENGL;
#endif
    m_Window = SDL_CreateWindow(SZ_DEFAULT_APP_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768,
                                winFlags);
    if (m_Window == nullptr) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        return;
    }
#if defined(USING_OPENGL)
    m_GLContext = SDL_GL_CreateContext(m_Window);
    if (m_GLContext == nullptr) {
        printf("SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        return;
    }
#endif
    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);
    if (m_Renderer == nullptr) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return;
    }
    m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 1024, 768);
    if (m_Texture == nullptr) {
        printf("SDL_CreateTexture failed: %s\n", SDL_GetError());
        return;
    }

}

#endif

#ifdef _WIN32

cGZWinMgrW32::cCanvasMessageTarget::~cCanvasMessageTarget() {}

uint32_t cGZWinMgrW32::cCanvasMessageTarget::AddRef() { return 0; }

uint32_t cGZWinMgrW32::cCanvasMessageTarget::Release() { return 0; }

bool cGZWinMgrW32::cCanvasMessageTarget::QueryInterface(uint32_t uiClsId,
                                                        void **ppvObj) {
    return false;
}

cGZWinMgrW32::cMouseMessageTarget::~cMouseMessageTarget() {}

uint32_t cGZWinMgrW32::cMouseMessageTarget::AddRef() { return 0; }

uint32_t cGZWinMgrW32::cMouseMessageTarget::Release() { return 0; }

bool cGZWinMgrW32::cMouseMessageTarget::QueryInterface(uint32_t uiClsId,
                                                       void **ppvObj) {
    return false;
}

cGZWinMgrW32::cKeyMessageTarget::~cKeyMessageTarget() {}

uint32_t cGZWinMgrW32::cKeyMessageTarget::AddRef() { return 0; }

uint32_t cGZWinMgrW32::cKeyMessageTarget::Release() { return 0; }

bool cGZWinMgrW32::cKeyMessageTarget::QueryInterface(uint32_t uiClsId,
                                                     void **ppvObj) {
    return 0;
}

#endif