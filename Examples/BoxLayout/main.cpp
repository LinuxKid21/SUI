#include <SUI/SUI.hpp>


int main() {
    sui::initiateSUI();
    const int window_width = 700;
    const int window_height = 300;

    sui::Theme theme;
    theme.loadDefaults("arial.ttf");
    
    // create a box running horizontally
    auto *hbox = new sui::BoxLayout(theme, sui::BoxLayout::HORIZONTAL);
    
    // All children will fit inside a 500x100 box garunteed
    hbox->setSize(sf::Vector2f(500,100));
    
    // center to the middle of the screen
    hbox->setPosition(sf::Vector2f(window_width/2, window_height/2));
    hbox->setOrigin(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    
    // create a button to add to hbox
    auto *button = new sui::Button(theme);
    button->setText("Add");
    button->setTextAlign(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    
    // for fun make the added boxes spell "hbox!!!!!!!!"
    const char *str = "hbox!";
    const char *iter = str;
    button->setOnClickedDown([&hbox, &theme, &iter](){
        auto *b = new sui::Button(theme);
        b->setText(std::string(1, *iter)); 
        b->setTextAlign(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
        
        // add button that will help spell "hbox!" to the hbox
        hbox->addChild(b);
        
        // whenver a child is added the layout must be recalculated
        hbox->layoutChanged();
        if(*iter != '!') {
            iter++;
        }
    });
    
    // add the button to out hbox
    hbox->addChild(button);
    
    // make the button occupy 3 times the width of the other buttons.
    // sui::BoxLayout::ABSOLUTE alternatively sets width in pixels so it can always
    // be a fixed amount of pixels
    hbox->setChildSize(button, sf::Vector2f(3,1), sui::BoxLayout::RELATIVE, sui::BoxLayout::RELATIVE);
    
    hbox->layoutChanged();


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
