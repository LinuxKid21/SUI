#include <SUI/LuaBindings.hpp>
#include <SUI/Button.hpp>
#include <SUI/CheckBox.hpp>


namespace sui {
    LuaGUI::LuaGUI() {
        L = luaL_newstate();
        
        lua_pushlightuserdata(L, this);
        lua_setglobal(L, "__gui");
        
        giveLuaFunc(L, "addButton", addButton);
        giveLuaFunc(L, "addCheckBox", addCheckBox);
        giveLuaFunc(L, "setPosition", setPosition);
        giveLuaFunc(L, "setSize", setSize);
        giveLuaFunc(L, "setProperty", setProperty);
        giveLuaFunc(L, "setProperties", setProperties);
        giveLuaFunc(L, "setOrigin", setOrigin);
        giveLuaFunc(L, "destroyWidget", destroyWidget);
    }
    LuaGUI::~LuaGUI() {
        for(auto &w : widgets) delete w;
        for(auto &f : fonts) delete f.second;
        lua_close(L);
    }
    
    void LuaGUI::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        for(auto &w : widgets) target.draw(*w, states);
    }
    void LuaGUI::update() {
        for(auto &w : widgets) w->update();
    }
    void LuaGUI::handleInput(sf::Event e) {
        for(auto &w : widgets) w->handleInput(e);
    }
    
    // -------------------------static funcs-------------------------
    int LuaGUI::addButton(lua_State *L) {
        checkArgCount(L, 0);
        
        Widget *w = new Button();
        getGUI(L)->widgets.push_back(w);
        lua_pushlightuserdata(L, w);
        return 1;
    }
    int LuaGUI::addCheckBox(lua_State *L) {
        checkArgCount(L, 0);
        
        Widget *w = new CheckBox();
        getGUI(L)->widgets.push_back(w);
        lua_pushlightuserdata(L, w);
        return 1;
    }
    
    int LuaGUI::setPosition(lua_State *L) {
        checkArgCount(L, 2);
        Widget *w = getWidgetArg(L, 1);
        sf::Vector2f v = getVecArg(L, 2);
        w->setPosition(v);
        return 0;
    }
    int LuaGUI::setSize(lua_State *L) {
        checkArgCount(L, 2);
        Widget *w = getWidgetArg(L, 1);
        sf::Vector2f v = getVecArg(L, 2);
        w->setSize(v);
        return 0;
    }
    int LuaGUI::setOrigin(lua_State *L) {
        checkArgCount(L, 2);
        Widget *w = getWidgetArg(L, 1);
        sf::Vector2<ORIGIN> o = getOriginArgVec(L, 2);
        w->setOrigin(o.x, o.y);
        return 0;
    }
    int LuaGUI::setProperty(lua_State *L) {
        checkArgCount(L, 2);
        Widget *w = getWidgetArg(L, 1);
        std::string name;
        Property p;
        getPropArg(L, 2, name, p);
        w->setProperty(name, p);
        return 0;
    }
    int LuaGUI::setProperties(lua_State *L) {
        checkArgCount(L, 2);
        Widget *w = getWidgetArg(L, 1);
        // second arg is table of properties
        
        lua_pushnil(L);
        while(lua_next(L, -2) != 0)
        {
            std::string name;
            Property p;
            getPropArg(L, -1, name, p);
            w->setProperty(name, p);
            lua_pop(L, 1);
        }
        return 0;
    }
    int LuaGUI::destroyWidget(lua_State *L) {
        checkArgCount(L, 1);
        LuaGUI *gui = getGUI(L);
        Widget *w = getWidgetArg(L, 1);
        for(auto iter = gui->widgets.begin(); iter != gui->widgets.end(); iter++) {
            if(*iter == w) {
                delete *iter;
                gui->widgets.erase(iter);
                lua_pushboolean(L, true);
                return 1;
            }
        }
        lua_pushboolean(L, false);
        return 1;
    }
}