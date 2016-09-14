#include <SUI/SUI.hpp>
#include <memory>

#include <iostream>
int main() {
    const int window_width = 500;
    const int window_height = 700;
    
    auto *button = new sui::Button();
    sf::Font font = sf::Font();
    font.loadFromFile("arial.ttf");
    button->setProperty<sf::Font *>("font", &font);
    button->setProperty<sf::String>("text", "Hello World");
    button->setProperty<float>("fontSize", 12);
    button->setProperty("fillColor", sf::Color(255,255,255,255));
    button->setProperty("fillColorHovered", sf::Color(200,200,200,255));
    button->setProperty("fillColorClicked", sf::Color(100,100,100,255));
    button->setProperty("textColor", sf::Color(0,0,0,255));
    button->setProperty("textAlignX", sui::ORIGIN_MIDDLE);
    button->setProperty("textAlignY", sui::ORIGIN_MIDDLE);
    button->setProperty("onClickedDown", sui::Property::makeFunc([](){
        std::cout << "Hello World" << "\n";
    }));
    button->setOrigin(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    button->setPosition(sf::Vector2f(window_width/2, window_height/2));
    button->setSize(sf::Vector2f(100,40));

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "SUI demo", sf::Style::Default, settings);
    window.setFramerateLimit(30);
    
    
    sf::Clock deltaClock;
    float red = 0;
    float reverse = 1;
    while (window.isOpen()) {
        // handle events.
    	sf::Event event;
    	while (window.pollEvent(event)) {
    		if (event.type == sf::Event::Closed)
    			window.close();

            button->handleInput(event);
    	}
        float dt = deltaClock.restart().asSeconds()*reverse;
        button->setPosition(button->getLocalPosition()+sf::Vector2f(20*dt, 0));
        button->setSize(button->getSize()+sf::Vector2f(10*dt, 10*dt));
        button->setProperty("textColor", sf::Color(red,0,0,255));
        red += dt*50;
        if(red < 0) {
            red = 0;
            reverse = 1;
        }
        if(red > 255) {
            red = 255;
            reverse = -1;
        }
        

    	window.clear();
        button->update();
        window.draw(*button);
    	window.display();
    }

    delete button;
}
