#include <SUI/SUI.hpp>
#include <memory>

#include <iostream>
int main() {
    const int window_width = 500;
    const int window_height = 700;
    
    // properties can be stored outside of the widget
    // here it serves as a handy 
    sui::Property BoxAbs = sui::Property::make(sui::BoxLayout::ABSOLUTE);

    sui::Container *vbox = new sui::BoxLayout();
    sf::Font font = sf::Font();
    font.loadFromFile("arial.ttf");
    vbox->setProperty("font", sui::Property::make(&font));
    vbox->setProperty("outlineThickness", sui::Property::make(2.f));
    vbox->setProperty("fillColor", sui::Property::make(sf::Color(255,255,255,255)));
    vbox->setProperty("fillColorHovered", sui::Property::make(sf::Color(200,200,200,255)));
    vbox->setProperty("fillColorClicked", sui::Property::make(sf::Color(100,100,100,255)));
    vbox->setProperty("textColor", sui::Property::make(sf::Color(0,0,0,255)));
    vbox->setProperty("outlineColor", sui::Property::make(sf::Color(0,255,0,255)));
    vbox->setProperty("fontSize", sui::Property::make(20.f));
    vbox->setProperty("textAlignX", sui::Property::make(sui::ORIGIN_MIDDLE));
    vbox->setProperty("textAlignY", sui::Property::make(sui::ORIGIN_MIDDLE));
    vbox->setProperty("padding", sui::Property::make(5.f));
    vbox->setProperty("direction", sui::Property::make(sui::BoxLayout::VERTICAL));
    
    
    
    vbox->setOrigin(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    vbox->setPosition(sf::Vector2f(window_width/2, window_height/2));
    vbox->setSize(sf::Vector2f(200,400));

    auto *button = vbox->addChild(new sui::Button());
    button->setProperty("text", sui::Property::make<sf::String>("Hello World"));
    button->setProperty("onClickedDown", sui::Property::makeFunc([](){
        std::cout << "Hello World" << "\n";
    }));
    
    auto combo_box = vbox->addChild(new sui::ComboBox());
    combo_box->setProperty("item0", sf::String("Apple"));
    combo_box->setProperty("item1", sf::String("Bannana"));
    combo_box->setProperty("item2", sf::String("Pie"));
    combo_box->setProperty("item3", sf::String("-------"));
    combo_box->setProperty("onPropertyChanged", sui::Property::makeFunc([&combo_box](){
        std::cout << combo_box->getProperty("item").as<sf::String>().toAnsiString() << "\n";
    }));
    
    auto *text_field = vbox->addChild(new sui::TextField());
    
    auto *text = vbox->addChild(new sui::Text());
    text->setProperty("text", sui::Property::make<sf::String>("asd..."));
    text->setProperty("textColor", sui::Property::make(sf::Color(255,255,255,255)));
    text->setProperty("boxSize", sui::Property::make(sf::Vector2f(1, 20)));
    text->setProperty("scaleTypeY", BoxAbs);
    
    
    text_field->setProperty("onChanged", sui::Property::makeFunc([&text_field, &text](){
        text->setProperty("text", text_field->getProperty("text"));
        text->update();
    }));
    text_field->setProperty("text", sui::Property::make<sf::String>("type in me!"));
    text_field->setProperty("boxSize", sui::Property::make(sf::Vector2f(1, 30)));
    text_field->setProperty("scaleTypeY", BoxAbs);
    
    
    auto *slider = vbox->addChild(new sui::Slider());
    
    // horizontal is default but vbox also has a property "direction" that this will erroneously try to read from
    slider->setProperty("direction", sui::Property::make(sui::Slider::HORIZONTAL));
    slider->setProperty("boxSize", sui::Property::make(sf::Vector2f(1, 30)));
    slider->setProperty("scaleTypeY", BoxAbs);
    slider->setProperty("max", sui::Property::make(1.f));
    slider->setProperty("min", sui::Property::make(-1.f));
    slider->setProperty("onValueChanged", sui::Property::makeFunc([&slider]() {
        std::cout << slider->getProperty("value").as<float>() << "\n";
    }));
    
    
    auto *check_box = vbox->addChild(new sui::CheckBox());
    check_box->setProperty("boxSize", sui::Property::make(sf::Vector2f(30, 30)));
    check_box->setProperty("scaleTypeX", BoxAbs);
    check_box->setProperty("scaleTypeY", BoxAbs);
    check_box->setProperty("outlineThickness", sui::Property::make(2.f));
    check_box->setProperty("onToggledOn", sui::Property::makeFunc([](){
        std::cout << "TOGGLED ON\n";
    }));
    check_box->setProperty("onToggledOff", sui::Property::makeFunc([](){
        std::cout << "TOGGLED OFF\n";
    }));
    
    
    sf::Texture sfml_texture;
    sfml_texture.loadFromFile("sfml_icon.png");
    auto *image = vbox->addChild(new sui::Image());
    image->setProperty("scaleTypeX", BoxAbs);
    image->setProperty("scaleTypeY", BoxAbs);
    image->setProperty("boxSize", sui::Property::make(sf::Vector2f(vbox->getSize().x, vbox->getSize().x)));
    image->setProperty("texture", sui::Property::make(&sfml_texture));
    



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
        vbox->update();
        window.draw(*vbox);
    	window.display();
    }

    delete vbox;
}
