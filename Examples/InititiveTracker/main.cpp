#include <SUI/SUI.hpp>
#include <memory>

#include "TableLayout.hpp"
#include <iostream>

int main() {
    sui::initiateSUI();
    sui::Theme::registerType("sui::BoxLayout", "BoxLayout");
    int window_width = 700;
    int window_height = 500;

    // Create a theme. This is shared by all Widgets and must remain in scope
    // for the duration of the application
    sui::Theme theme;
    theme.loadDefaults("arial.ttf");
    theme.setNumberProperty("sui::Widget.outlineThickness", 0);
    
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
    
	sf::View view(sf::FloatRect(0, 0, window_width, window_height));
    
    sui::BoxLayout *vbox = new sui::BoxLayout(theme, sui::BoxLayout::VERTICAL);
    sui::BoxLayout *hbox = new sui::BoxLayout(theme, sui::BoxLayout::HORIZONTAL);
    vbox->addChild(hbox);
    
    sui::Button *sort_button = new sui::Button(theme);
    sort_button->setText("Sort");
    sort_button->setNumberProperty("fontSize", 16);
    sort_button->updateTheme();
    sort_button->setTextAlign(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    hbox->addChild(sort_button);
    hbox->setChildSize(sort_button, sf::Vector2f(75,1), sui::BoxLayout::ABSOLUTE, sui::BoxLayout::RELATIVE);
    
    sui::CheckBox *check_box = new sui::CheckBox(theme);
    check_box->setNumberProperty("outlineThickness", 2);
    check_box->updateTheme();
    hbox->addChild(check_box);
    // 25 is calculated for lack of a better method
    hbox->setChildSize(check_box, sf::Vector2f(25,25), sui::BoxLayout::ABSOLUTE, sui::BoxLayout::ABSOLUTE);
    
    sui::FrameLayout *auto_sort_text_frame = new sui::FrameLayout(theme);
    sui::Text *auto_sort_text = new sui::Text(theme);
    auto_sort_text->setText("Auto Sort");
    auto_sort_text_frame->addChild(auto_sort_text);
    auto_sort_text_frame->setChildSize(auto_sort_text, sf::Vector2f(1,.5), sui::FrameLayout::RELATIVE, sui::FrameLayout::RELATIVE);
    auto_sort_text_frame->setChildPosition(auto_sort_text, sf::Vector2f(0,.5), sui::FrameLayout::RELATIVE, sui::FrameLayout::RELATIVE);
    
    hbox->addChild(auto_sort_text_frame);
    
    
    sui::ColorBox *divider = new sui::ColorBox(theme);
    vbox->addChild(divider);
    vbox->setChildSize(divider, sf::Vector2f(1,3), sui::BoxLayout::RELATIVE, sui::BoxLayout::ABSOLUTE);
    
    
    
    TableLayout *table = new TableLayout(theme);
    table->setOnAddClicked([table, vbox](){
        table->addColumn();
        vbox->setSize(sf::Vector2f(table->getUsedWidth(),138));
        vbox->layoutChanged();
    });
    
    table->setLineThickness(3);
    
    
    vbox->setPosition(sf::Vector2f(window_width/2,window_height/2));
    vbox->setOrigin(sui::ORIGIN_MIDDLE, sui::ORIGIN_MIDDLE);
    vbox->setSize(sf::Vector2f(table->getUsedWidth(),138));
    vbox->addChild(table);
    vbox->setChildSize(table, sf::Vector2f(1, 100), sui::BoxLayout::RELATIVE, sui::BoxLayout::ABSOLUTE);
    vbox->layoutChanged();

    sort_button->setOnClickedDown([table](){
        table->sort();
    });
    
    check_box->setOnToggledOn([table](){
        table->setAutoSort(true);
    });
    check_box->setOnToggledOff([table](){
        table->setAutoSort(false);
    });

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
                
                // std::cout << window_width << "\n";
                vbox->setPosition(sf::Vector2f(window_width/2,window_height/2));
                vbox->layoutChanged();
            }
            vbox->handleInput(event);
    	}

    	window.clear(sf::Color(64,64,64,255));
        window.draw(*vbox);
    	window.display();
    }

    delete vbox;
}
