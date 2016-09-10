#include <SUI/SUI.hpp>


int main() {
    const int window_width = 700;
    const int window_height = 300;

    
    // create a box running horizontally
    auto *hbox = new sui::BoxLayout();
    sf::Font font = sf::Font();
    font.loadFromFile("arial.ttf");
    hbox->setProperty("font", sui::Property::make(&font));
    hbox->setProperty("outlineThickness", sui::Property::make(4.f));
    hbox->setProperty("fillColor", sui::Property::make(sf::Color(255,255,255,255)));
    hbox->setProperty("fillColorHovered", sui::Property::make(sf::Color(200,200,200,255)));
    hbox->setProperty("fillColorClicked", sui::Property::make(sf::Color(100,100,100,255)));
    hbox->setProperty("textColor", sui::Property::make(sf::Color(0,0,0,255)));
    hbox->setProperty("outlineColor", sui::Property::make(sf::Color(0,255,0,255)));
    hbox->setProperty("fontSize", sui::Property::make(24.f));
    hbox->setProperty("textAlignX", sui::Property::make(sui::ORIGIN_MIDDLE));
    hbox->setProperty("textAlignY", sui::Property::make(sui::ORIGIN_MIDDLE));
    hbox->setProperty("padding", sui::Property::make(5.f));
    
    // All children will fit inside a 500x100 box garunteed
    hbox->setSize(sf::Vector2f(500,100));
    
    // center to the middle of the screen
    hbox->setPosition(sf::Vector2f(window_width/2, window_height/2));
    hbox->setOrigin(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    
    // create a button to add to hbox
    auto *button = new sui::Button();
    button->setProperty("text", sui::Property::make<sf::String>("Add"));
    
    // tell any box owning this button that we want 3 times the space of any other button!
    // the "scaleTypeX" (and similiar Y) property can be set to sui::BoxLayout::ABSOLUTE
    // then these values are absolute so we can garuntee this button will have say 50 pixels
    button->setProperty("boxSize", sui::Property::make(sf::Vector2f(3,1)));
    
    // for fun make the added boxes spell "hbox!!!!!!!!"
    const char *str = "hbox!";
    const char *iter = str;
    button->setProperty("onClickedDown", sui::Property::makeFunc([&hbox, &iter](){
        auto *b = new sui::Button();
        b->setProperty("text", sui::Property::make<sf::String>(std::string(1, *iter)));
        
        // add button that will help spell "hbox!" to the hbox
        hbox->addChild(b);
        
        // whenver a child is added the layout must be recalculated
        hbox->update();
        if(*iter != '!') {
            iter++;
        }
    }));
    
    // add the button to out hbox
    hbox->addChild(button);

    
    hbox->update();


    sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "SUI demo", sf::Style::Default);

    while (window.isOpen()) {
    	sf::Event event;
    	while (window.pollEvent(event)) {
    		if (event.type == sf::Event::Closed)
    			window.close();

            // this will automatically call this for all of it's children
            hbox->handleInput(event);
    	}
    	window.clear();
        
        // this will automatically call this for all of it's children
        window.draw(*hbox);
    	window.display();
    }

    // this will recursively delete all children
    // if that's unwanted then you must use removeChild on all the children
    delete hbox;
}
