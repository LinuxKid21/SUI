#include <SUI/SUI.hpp>
#include <memory>

#include <iostream>
int main() {
    sui::initiateSUI();
    const int window_width = 500;
    const int window_height = 700;

    // Create a theme. This is shared by all Widgets and must remain in scope
    // for the duration of the application
    sui::Theme theme;
    theme.loadDefaults("arial.ttf");


        
    sui::BoxLayout *vbox = new sui::BoxLayout(theme, sui::BoxLayout::VERTICAL);
    vbox->setOrigin(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    vbox->setPosition(sf::Vector2f(window_width/2, window_height/2));
    vbox->setSize(sf::Vector2f(200,400));

    // create a button with the theme from above
    auto *button = static_cast<sui::Button *>(vbox->addChild(new sui::Button(theme)));
    button->setText("Hello World");
    button->setTextAlign(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    button->setOnClickedDown([](){
        std::cout << "Hello World" << "\n";
    });
    
    auto *text_field = static_cast<sui::TextField *>(vbox->addChild(new sui::TextField(theme)));
    
    auto *text = static_cast<sui::Text *>(vbox->addChild(new sui::Text(theme)));
    vbox->setChildSize(text, sf::Vector2f(1,20), sui::BoxLayout::RELATIVE, sui::BoxLayout::ABSOLUTE);
    
    
    text_field->setOnChanged([&text_field, &text](){
        text->setText(text_field->getText());
    });
    text_field->setText("type in me!");
    vbox->setChildSize(text_field, sf::Vector2f(1,30), sui::BoxLayout::RELATIVE, sui::BoxLayout::ABSOLUTE);
    
    
    auto *slider = static_cast<sui::Slider *>(vbox->addChild(new sui::Slider(theme, sui::Slider::HORIZONTAL)));
    vbox->setChildSize(slider, sf::Vector2f(1,30), sui::BoxLayout::RELATIVE, sui::BoxLayout::ABSOLUTE);
    slider->setRange(0,100);
    slider->setOnValueChanged([&slider](){
        std::cout << slider->getValue() << "\n";
    });
    
    
    auto *check_box = static_cast<sui::CheckBox *>(vbox->addChild(new sui::CheckBox(theme)));
    vbox->setChildSize(check_box, sf::Vector2f(30,30), sui::BoxLayout::ABSOLUTE, sui::BoxLayout::ABSOLUTE);
    check_box->setOnToggledOn([](){
        std::cout << "TOGGLED ON\n";
    });
    check_box->setOnToggledOff([](){
        std::cout << "TOGGLED OFF\n";
    });
    
    
    sf::Texture sfml_texture;
    sfml_texture.loadFromFile("sfml_icon.png");
    auto *image = static_cast<sui::Image *>(vbox->addChild(new sui::Image(theme, sfml_texture)));
    vbox->setChildSize(image, sf::Vector2f(vbox->getSize().x,vbox->getSize().x), sui::BoxLayout::ABSOLUTE, sui::BoxLayout::ABSOLUTE);
    
    vbox->layoutChanged();



    sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "SUI demo", sf::Style::Default);

    while (window.isOpen()) {
        // handle events.
    	sf::Event event;
    	while (window.pollEvent(event)) {
    		if (event.type == sf::Event::Closed)
    			window.close();

            vbox->handleInput(event);
    	}

    	window.clear();
        window.draw(*vbox);
    	window.display();
    }

    delete vbox;
}
