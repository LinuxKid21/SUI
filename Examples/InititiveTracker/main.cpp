#include <SUI/SUI.hpp>
#include <memory>

#include "TableLayout.hpp"
#include <iostream>
#include <string>


int main() {
    int window_width = 700;
    int window_height = 500;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
    
	sf::View view(sf::FloatRect(0, 0, window_width, window_height));
    
    sui::Property BoxAbs = sui::Property::make(sui::BoxLayout::ABSOLUTE);
    
    ThemeReader reader;
    
    sui::BoxLayout *vbox = new sui::BoxLayout();
    reader.setWidgetProps(vbox, "all");
    vbox->setProperty("padding", sui::Property::make(5.f));
    vbox->setProperty("direction", sui::Property::make(sui::BoxLayout::VERTICAL));
    
    vbox->setPosition(sf::Vector2f(window_width/2,window_height/2));
    vbox->setOrigin(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    
    
    
    
    sui::BoxLayout *hbox = new sui::BoxLayout();
    hbox->setProperty("direction", sui::Property::make(sui::BoxLayout::HORIZONTAL));
    vbox->addChild(hbox);
    
    auto *sort_button = hbox->addChild(new sui::Button());
    reader.setWidgetProps(sort_button, "sortButton");
    sort_button->setProperty("text", sui::Property::make<sf::String>("Sort"));
    sort_button->setProperty("boxSize", sui::Property::make(sf::Vector2f(75, 1)));
    sort_button->setProperty("scaleTypeX", BoxAbs);
    
    
    auto *check_box = hbox->addChild(new sui::CheckBox());
    reader.setWidgetProps(sort_button, "autoSortCheck");
    check_box->setProperty("outlineThickness", 2.f);
    // 25 is calculated for lack of a better method
    check_box->setProperty("boxSize", sf::Vector2f(25, 25));
    check_box->setProperty("scaleTypeX", BoxAbs);
    check_box->setProperty("scaleTypeY", BoxAbs);
    
    sui::FrameLayout *auto_sort_text_frame = static_cast<sui::FrameLayout *>(hbox->addChild(new sui::FrameLayout()));
    auto *auto_sort_text = auto_sort_text_frame->addChild(new sui::Text());
    reader.setWidgetProps(auto_sort_text, "autoSortText");
    auto_sort_text->setProperty("text", sui::Property::make<sf::String>("Auto Sort"));
    auto_sort_text_frame->setProperty("frameSize", sf::Vector2f(1,.5));
    auto_sort_text_frame->setProperty("framePos", sf::Vector2f(0,.5));
    
    auto *divider = vbox->addChild(new sui::ColorBox());
    reader.setWidgetProps(divider, "divider");
    divider->setProperty("boxSize", sf::Vector2f(1, 3));
    divider->setProperty("scaleTypeY", BoxAbs);
    
    
    
    TableLayout *table = new TableLayout(reader);
    
    reader.setWidgetProps(table, "table");
    table->setProperty("onChanged", sui::Property::makeFunc([table, vbox](){
        vbox->setSize(sf::Vector2f(table->getUsedWidth(),138));
    }));
    
    table->setProperty("boxSize", sui::Property::make(sf::Vector2f(1, 100)));
    table->setProperty("scaleTypeX", sui::Property::make(sui::BoxLayout::RELATIVE));
    table->setProperty("scaleTypeY", sui::Property::make(sui::BoxLayout::ABSOLUTE));
    vbox->addChild(table);
    
    sort_button->setProperty("onClickedDown", sui::Property::makeFunc([table](){
        table->sort();
    }));
    
    check_box->setProperty("onToggledOn", sui::Property::makeFunc([table](){
        table->setAutoSort(true);
    }));
    check_box->setProperty("onToggledOff", sui::Property::makeFunc([table](){
        table->setAutoSort(false);
    }));
    
    vbox->setSize(sf::Vector2f(table->getUsedWidth(),138));

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
            }
            vbox->handleInput(event);
    	}

    	window.clear(sf::Color(64,64,64,255));
        vbox->update();
        window.draw(*vbox);
    	window.display();
    }

    delete vbox;
}
