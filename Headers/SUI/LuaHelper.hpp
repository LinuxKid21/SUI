#pragma once

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}
#include <string>

namespace sui {
    void luaPrintError(lua_State *L);
    int loadLuaFile(lua_State *L, std::string file);

    void giveLuaFunc(lua_State *L, const char *lua_name, int(*func)(lua_State *L));
}