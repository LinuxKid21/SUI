#pragma once
#include <SFML/Graphics.hpp>
#include "LuaHelper.hpp"
#include "Widget.hpp"

namespace sui {
    class LuaGUI : public sf::Drawable {
    public:
        LuaGUI();
        ~LuaGUI();
        
        void loadFile(const char *file) {
            sui::loadLuaFile(L, file);
        }
        
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void update();
        void handleInput(sf::Event e);
        std::map<std::string, std::function<void()> > funcs;
        lua_State *L;
    private:
        static void checkArgCount(lua_State *L, lua_Integer v) {
            if(lua_gettop(L) != v) {
                luaL_error(L, "requires %I arg", v);
            }
        }
        static sf::Vector2<ORIGIN> getOriginArgVec(lua_State *L, int index) {
            static auto badArg = [](lua_State *s, int i){
                luaL_error(s, "arg must be table of form {\"x origin\",\"y origin\"}");
            };
            if(!lua_istable(L, index)) badArg(L, index);
            ORIGIN x;
            ORIGIN y;
            lua_pushvalue(L, index);
            lua_pushnil(L);
            int i = 0;
            while(lua_next(L, -2) != 0)
            {
                if(!lua_isinteger(L, -2)) badArg(L, index);
                if(i == 0)      x = getOriginArg(L, -1);
                else if(i == 1) y = getOriginArg(L, -1);
                else badArg(L, index);
                i++;
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
            if(i != 2) badArg(L, index);
            return sf::Vector2<ORIGIN>(x, y);
        }
        static sf::Vector2f getVecArg(lua_State *L, int index) {
            static auto badArg = [](lua_State *s, int i){
                luaL_error(s, "arg must be table of form {x,y}");
            };
            if(!lua_istable(L, index)) badArg(L, index);
            float x;
            float y;
            lua_pushvalue(L, index);
            lua_pushnil(L);
            int i = 0;
            while(lua_next(L, -2) != 0)
            {
                if(!lua_isinteger(L, -2)) badArg(L, index);
                if(i == 0)      x = lua_tonumber(L, -1);
                else if(i == 1) y = lua_tonumber(L, -1);
                else badArg(L, index);
                i++;
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
            if(i != 2) badArg(L, index);
            return sf::Vector2f(x, y);
        }
        static sf::Color getColorArg(lua_State *L, int index) {
            static auto badArg = [](lua_State *s, int i){
                luaL_error(s, "arg must be table of form {x,y}");
            };
            if(!lua_istable(L, index)) badArg(L, index);
            float r;
            float g;
            float b;
            float a = 255;
            lua_pushvalue(L, index);
            lua_pushnil(L);
            int i = 0;
            while(lua_next(L, -2) != 0)
            {
                if(!lua_isinteger(L, -2)) badArg(L, index);
                if(i == 0)      r = lua_tonumber(L, -1);
                else if(i == 1) g = lua_tonumber(L, -1);
                else if(i == 2) b = lua_tonumber(L, -1);
                else if(i == 3) a = lua_tonumber(L, -1);
                else badArg(L, index);
                i++;
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
            if(i != 3 && i != 4) badArg(L, index);
            return sf::Color(r, g, b, a);
        }
        static std::function<void()> getFuncArg(lua_State *L, int index) {
            static auto badArg = [](lua_State *s, int i){
                luaL_error(s, "arg must string refering to an action defined in the application(e.g. \"start\" to start the game)");
            };
            if(!lua_isstring(L, index)) badArg(L, index);
            LuaGUI *gui = getGUI(L);
            std::string s = lua_tostring(L, index);
            if(gui->funcs.find(s) == gui->funcs.end()) badArg(L, index);
            
            return gui->funcs[s];
        }
        static void getPropArg(lua_State *L, int index, std::string &name, Property &p) {
            static auto badArg = [](lua_State *s, int i){
                luaL_error(s, "arg must be table of form {\"type\", \"prop name\", value}");
            };
            if(!lua_istable(L, index)) badArg(L, index);
            std::string type;
            lua_pushvalue(L, index);
            lua_pushnil(L);
            int i = 0;
            while(lua_next(L, -2) != 0)
            {
                if(!lua_isinteger(L, -2)) badArg(L, index);
                if(i == 0)      type = lua_tostring(L, -1);
                else if(i == 1) name = lua_tostring(L, -1);
                else if(i == 2) {
                    if(type == "number")
                        p = Property::make<float>(luaL_checknumber(L, -1));
                    else if(type == "integer")
                        p = Property::make<int>(luaL_checkinteger(L, -1));
                    else if(type == "vec2d")
                        p = Property::make<sf::Vector2f>(getVecArg(L, -1));
                    else if(type == "color")
                        p = Property::make<sf::Color>(getColorArg(L, -1));
                    else if(type == "font")
                        p = Property::make<sf::Font *>(getFontArg(L, -1));
                    else if(type == "string")
                        p = Property::make<sf::String>(luaL_checkstring(L, -1));
                    else if(type == "origin")
                        p = Property::make<ORIGIN>(getOriginArg(L, -1));
                    else if(type == "function")
                        p = Property::makeFunc(getFuncArg(L, -1));
                    else
                        luaL_error(L, "unkown type \"%s\". Types are number, integer, vec2d, color", type.c_str());
                }
                else badArg(L, index);
                i++;
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
            if(i != 3) badArg(L, index);
        }
        static ORIGIN getOriginArg(lua_State *L, int index) {
            if(!lua_isstring(L, index)) luaL_error(L, "arg must be string");
            std::string s = lua_tostring(L, index);
            if(s == "start")       return ORIGIN_START;
            else if(s == "middle") return ORIGIN_MIDDLE;
            else if(s == "end")    return ORIGIN_END;
            else luaL_error(L, "arg must be one of \"start\" \"middle\" or \"end\"");
        }
        static Widget *getWidgetArg(lua_State *L, int index) {
            if(!lua_islightuserdata(L, index)) luaL_error(L, "arg must be lightuserdata");
            return static_cast<Widget *>(lua_touserdata(L, index));
        }
        static sf::Font *getFontArg(lua_State *L, int index) {
            if(!lua_isstring(L, index)) luaL_error(L, "arg must be string of font name");
            
            LuaGUI *gui = getGUI(L);
            auto &fonts = gui->fonts;
            std::string str = luaL_checkstring(L, index);
            if(fonts.find(str) == fonts.end()) {
                fonts[str] = new sf::Font();
                fonts[str]->loadFromFile(str);
            }
            
            return fonts[str];
        }
        static LuaGUI *getGUI(lua_State *L) {
            lua_getglobal(L, "__gui");
            LuaGUI *gui = static_cast<LuaGUI *>(lua_touserdata(L, -1));
            lua_pop(L, 1);
            return gui;
        }
        
        static int addWidget(lua_State *L, Widget *w);
        static int setPosition(lua_State *L);
        static int setSize(lua_State *L);
        static int setOrigin(lua_State *L);
        static int setProperty(lua_State *L);
        static int setProperties(lua_State *L);
        static int destroyWidget(lua_State *L);
        std::vector<Widget *> widgets;
        std::map<std::string, sf::Font *> fonts;
    };
}