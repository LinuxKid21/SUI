#include <SUI/LuaHelper.hpp>

#include <iostream>
#include <string>

namespace sui {
    void luaPrintError(lua_State *L) {
        // The error message is on top of the stack.
        // Fetch it, print it and then pop it off the stack.
        const char* message = lua_tostring(L, -1);
        std::cout << message << "\n";
        lua_pop(L, 1);
        std::exit(-1);
    }


    int loadLuaFile(lua_State *L, std::string file) {
        int result;
        result = luaL_loadfile(L, file.c_str());
        
        if ( result != LUA_OK ) {
            luaPrintError(L);
            return result;
        }
        
        result = lua_pcall(L, 0, LUA_MULTRET, 0);
        
        if ( result != LUA_OK ) {
            luaPrintError(L);
            return result;
        }
    }


    void giveLuaFunc(lua_State *L, const char *lua_name, int(*func)(lua_State *L)) {
        lua_pushcfunction(L, func);
        lua_setglobal(L, lua_name);
    }
}
