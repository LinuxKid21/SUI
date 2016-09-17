#include <SUI/SUI.hpp>
int main() {
    const int window_width = 700;
    const int window_height = 300;
    
    sui::LuaGUI lua_gui;
    lua_gui.funcs["test"] = [](){
        std::cout << "WHATEVER\n";
    };
    
    lua_gui.loadFile("main.lua");
    
    
    sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "SUI demo", sf::Style::Default);
    window.setFramerateLimit(30);
    
    while (window.isOpen()) {
        // handle events.
    	sf::Event event;
    	while (window.pollEvent(event)) {
    		if (event.type == sf::Event::Closed)
    			window.close();
            lua_gui.handleInput(event);
    	}
        lua_gui.update();
        
    	window.clear();
        window.draw(lua_gui);
    	window.display();
    }
    
}