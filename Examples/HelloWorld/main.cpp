#include <SUI/SUI.hpp>
#include <iostream>
int main() {
    const int window_width = 700;
    const int window_height = 300;

    // create a button with the theme from above
    auto button = new sui::Button();
    
    sf::Font font = sf::Font();
    font.loadFromFile("arial.ttf");
    button->setProperty("font", sui::Property::make(&font));
    button->setProperty("outlineThickness", sui::Property::make(2.f));
    button->setProperty("fillColor", sui::Property::make(sf::Color(255,255,255,255)));
    button->setProperty("fillColorHovered", sui::Property::make(sf::Color(200,200,200,255)));
    button->setProperty("fillColorClicked", sui::Property::make(sf::Color(100,100,100,255)));
    button->setProperty("textColor", sui::Property::make(sf::Color(0,0,0,255)));
    button->setProperty("outlineColor", sui::Property::make(sf::Color(0,255,0,255)));
    button->setProperty("fontSize", sui::Property::make(24.f));

    button->setProperty("text", sui::Property::make<sf::String>("Hello World"));

    // place the buttons in the very center of the screen
    button->setPosition(sf::Vector2f(window_width/2,window_height/2));
    
    // give the button a reasonable size
    button->setSize(sf::Vector2f(200,75));
    
    // set the origin of the button in the center. If it were in the default
    // (sui::ORIGIN_START for both args which means top left) then the top left
    // corner of the button would be in the center of the screen instead.
    button->setOrigin(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    
    // put the text in the middle.
    // sui::ORIGIN_START places the text at the left for x direction and top for y
    // sui::ORIGIN_END places the text at the right for x direction and bottom for y
    button->setProperty("textOriginX", sui::Property::make(sui::ORIGIN_MIDDLE));
    button->setProperty("textOriginY", sui::Property::make(sui::ORIGIN_MIDDLE));
    
    // after everything is set up tell the button to update the graphics.
    // this must be called after almost any function that makes a visible
    // difference on the screen. For efficiency many graphical parameters
    // can all be set before calculating the graphical components
    button->update();
    
    // set actions that the button will do upon certain events.
    // note: all functions that take a callback use an std::function
    // meaning you can use lambdas as done below
    button->setProperty("onEntered", sui::Property::makeFunc([](){
        std::cout << "Button Entered!\n";
    }));
    button->setProperty("onExited", sui::Property::makeFunc([](){
        std::cout << "Button Exited!\n";
    }));
    button->setProperty("onClickedDown", sui::Property::makeFunc([](){
        std::cout << "Button Clicked Down!\n";
    }));
    button->setProperty("onClickedUp", sui::Property::makeFunc([](){
        std::cout << "Button Clicked Up!\n";
    }));


    sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "SUI demo", sf::Style::Default);
    window.setFramerateLimit(30);
    
    while (window.isOpen()) {
        // handle events.
    	sf::Event event;
    	while (window.pollEvent(event)) {
    		if (event.type == sf::Event::Closed)
    			window.close();
            // All top-level elements must be handled here like this or they won't react to your input!
            // Children of a container shoud not be passed into here because they are automatically called by their parent
            button->handleInput(event);
    	}
        // clear the window so we are drawing on a clean slate
    	window.clear();
        
        // make sure any changed properties are updated before drawing.
        // note this should be done even in this case were we don't
        // change properties over time because button does(state as normal, clicked or hovered)
        button->update();
        
        // draw the button
        window.draw(*button);
        
        // put it on the screen
    	window.display();
    }

    // You must delete top-level containers. They will automatically delete all children recursivelly
    delete button;
}
