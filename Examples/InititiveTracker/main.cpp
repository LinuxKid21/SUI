#include <SUI/SUI.hpp>
#include <memory>

#include "TableLayout.hpp"
#include <iostream>

int main() {
    int window_width = 700;
    int window_height = 500;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
    
	sf::View view(sf::FloatRect(0, 0, window_width, window_height));
    
    sui::Property BoxAbs = sui::Property::make(sui::BoxLayout::ABSOLUTE);
    
    sui::BoxLayout *vbox = new sui::BoxLayout();
    sf::Font font = sf::Font();
    font.loadFromFile("arial.ttf");
    vbox->setProperty("font", sui::Property::make(&font));
    vbox->setProperty("outlineThickness", sui::Property::make(4.f));
    vbox->setProperty("fillColor", sui::Property::make(sf::Color(255,255,255,255)));
    vbox->setProperty("fillColorHovered", sui::Property::make(sf::Color(200,200,200,255)));
    vbox->setProperty("fillColorClicked", sui::Property::make(sf::Color(100,100,100,255)));
    vbox->setProperty("textColor", sui::Property::make(sf::Color(0,0,0,255)));
    vbox->setProperty("outlineColor", sui::Property::make(sf::Color(0,255,0,255)));
    vbox->setProperty("fontSize", sui::Property::make(24.f));
    vbox->setProperty("textAlignX", sui::Property::make(sui::ORIGIN_MIDDLE));
    vbox->setProperty("textAlignY", sui::Property::make(sui::ORIGIN_MIDDLE));
    vbox->setProperty("padding", sui::Property::make(5.f));
    vbox->setProperty("direction", sui::Property::make(sui::BoxLayout::VERTICAL));
    
    vbox->setPosition(sf::Vector2f(window_width/2,window_height/2));
    vbox->setOrigin(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    
    
    
    
    sui::BoxLayout *hbox = new sui::BoxLayout();
    hbox->setProperty("direction", sui::Property::make(sui::BoxLayout::HORIZONTAL));
    vbox->addChild(hbox);
    
    auto *sort_button = hbox->addChild(new sui::Button());
    sort_button->setProperty("text", sui::Property::make<sf::String>("Sort"));
    sort_button->setProperty("fontSize", sui::Property::make(16.f));
    sort_button->setProperty("boxSize", sui::Property::make(sf::Vector2f(75, 1)));
    sort_button->setProperty("scaleTypeX", BoxAbs);
    
    
    auto *check_box = hbox->addChild(new sui::CheckBox());
    check_box->setProperty("outlineThickness", sui::Property::make(2.f));
    // 25 is calculated for lack of a better method
    check_box->setProperty("boxSize", sui::Property::make(sf::Vector2f(25, 25)));
    check_box->setProperty("scaleTypeX", BoxAbs);
    check_box->setProperty("scaleTypeY", BoxAbs);
    
    sui::FrameLayout *auto_sort_text_frame = static_cast<sui::FrameLayout *>(hbox->addChild(new sui::FrameLayout()));
    auto *auto_sort_text = auto_sort_text_frame->addChild(new sui::Text());
    auto_sort_text->setProperty("text", sui::Property::make<sf::String>("Auto Sort"));
    auto_sort_text->setProperty("textColor", sui::Property::make(sf::Color(255,255,255,255)));
    auto_sort_text_frame->setProperty("frameSize", sui::Property::make(sf::Vector2f(1,.5)));
    auto_sort_text_frame->setProperty("framePos", sui::Property::make(sf::Vector2f(0,.5)));
    
    auto *divider = vbox->addChild(new sui::ColorBox());
    divider->setProperty("boxSize", sui::Property::make(sf::Vector2f(1, 3)));
    divider->setProperty("scaleTypeY", BoxAbs);
    divider->setProperty("fillColor", sui::Property::make(sf::Color(0,0,0,255)));
    
    
    
    TableLayout *table = new TableLayout();
    table->setProperty("onAdded", sui::Property::makeFunc([table, vbox](){
        vbox->setSize(sf::Vector2f(table->getUsedWidth(),138));
        vbox->update();
    }));
    
    table->setProperty("padding", sui::Property::make(3.f));
    
    
    vbox->setSize(sf::Vector2f(table->getUsedWidth(),138));
    table->setProperty("boxSize", sui::Property::make(sf::Vector2f(1, 100)));
    table->setProperty("scaleTypeX", sui::Property::make(sui::BoxLayout::RELATIVE));
    table->setProperty("scaleTypeY", sui::Property::make(sui::BoxLayout::ABSOLUTE));
    // vbox->setSize(sf::Vector2f(table->getUsedWidth(),138));
    vbox->addChild(table);
    vbox->update();

    sort_button->setProperty("onClickedDown", sui::Property::makeFunc([table](){
        table->sort();
    }));
    
    check_box->setProperty("onToggledOn", sui::Property::makeFunc([table](){
        table->setAutoSort(true);
    }));
    check_box->setProperty("onToggledOff", sui::Property::makeFunc([table](){
        table->setAutoSort(false);
    }));

    sf::RenderWindow window(sf::VideoMode(window_width, window_height, 32), "Inititive Tracker", sf::Style::Default, settings);
    window.setFramerateLimit(30);
	window.setView(view);

    while (window.isOpen()) {
        // handle events.
    	sf::Event event;
    	while (window.pollEvent(event)) {
    		if (event.type == sf::Event::Closed) {
    			window.close();
            } else if (event.type == sf::Event::Resized) {
                window_width = window.getSize().x;
                window_height = window.getSize().y;
                view = sf::View(sf::FloatRect(0, 0, window_width, window_height));
                window.setView(view);
                
                vbox->setPosition(sf::Vector2f(window_width/2,window_height/2));
                vbox->update();
            }
            vbox->handleInput(event);
    	}

    	window.clear(sf::Color(64,64,64,255));
        window.draw(*vbox);
    	window.display();
    }

    delete vbox;
}
