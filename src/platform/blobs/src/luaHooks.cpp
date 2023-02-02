#include "luaHooks.h"

#ifdef TARGET_GAME_sc4
DECL_EXTERN_C_FUNC(lua_open,0x005C4ECD)
DECL_EXTERN_C_FUNC(lua_resume,0x005BE223)
DECL_EXTERN_C_FUNC(lua_pcall,0x005B826E)
DECL_EXTERN_C_FUNC(lua_load,0x005B82E2)
DECL_EXTERN_C_FUNC(lua_dobuffer,0x005B9AE2)
#endif

void HookLuaLibraryFunctions() {
#ifdef TARGET_GAME_sc4
  HOOK_FUNC(lua_open)
  HOOK_FUNC(lua_resume)
  HOOK_FUNC(lua_pcall)
  HOOK_FUNC(lua_load)
  HOOK_FUNC(lua_dobuffer)
#endif
}
