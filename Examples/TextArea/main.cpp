#include <SUI/SUI.hpp>
#include <iostream>
int main() {
    const int window_width = 700;
    const int window_height = 300;

    // create a button with the theme from above
    auto text_area = new sui::MsgTextArea();
    
    sf::Font font = sf::Font();
    font.loadFromFile("arial.ttf");
    text_area->setProperty("font", &font);
    text_area->setProperty("outlineThickness", 2.f);
    text_area->setProperty("fillColor", sf::Color(255,255,255,255));
    text_area->setProperty("fillColorHovered", sf::Color(200,200,200,255));
    text_area->setProperty("fillColorClicked", sf::Color(100,100,100,255));
    text_area->setProperty("textColor", sf::Color(0,0,0,255));
    text_area->setProperty("outlineColor", sf::Color(0,255,0,255));
    text_area->setProperty("fontSize", 24.f);
    text_area->setProperty("interval", .02f);
    
    
    text_area->setProperty("color0", sf::Color(255,0,0,255));
    text_area->setProperty("color1", sf::Color(0,255,0,255));
    text_area->setProperty("file", sf::String("Message.msg"));
    
    

    text_area->setPosition(sf::Vector2f(window_width/2,window_height/2));
    text_area->setSize(sf::Vector2f(500,200));
    text_area->setOrigin(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);


    sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "SUI demo", sf::Style::Default);
    window.setFramerateLimit(30);
    
    while (window.isOpen()) {
        // handle events.
    	sf::Event event;
    	while (window.pollEvent(event)) {
    		if (event.type == sf::Event::Closed)
    			window.close();
            text_area->handleInput(event);
    	}
    	window.clear();
        text_area->update();
        window.draw(*text_area);
    	window.display();
    }

    delete text_area;
}
