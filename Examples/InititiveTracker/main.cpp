#include <SUI/SUI.hpp>
#include <memory>

#include "TableLayout.hpp"
#include <iostream>
#include <fstream>
#include <string>

struct ThemeReader {
    ThemeReader() {
        file.open("Theme.txt", std::ios::in);
    }
    void setWidgetProps(sui::Widget *w, const std::string section) {
        file.clear();
        file.seekg(0, std::ios::beg);
        
        std::string line;
        while(getline(file, line)) {
            if(line == "[" + section + "]")
                break;
        };
        while(getline(file, line)) {
            if(line.size() == 0) continue;
            if(line[0] == '#') continue; // comment
            if(line[0] == '[') break; // new section
            std::string propName;
            std::string propValue;
            bool foundName = false;
            for(char c : line) {
                if(c == '=') {
                    foundName = true;
                } else if(foundName) {
                    propValue += c;
                } else {
                    propName += c;
                }
            }
            assignProp(w, propName, propValue);
        }
    }
    void assignProp(sui::Widget *w, std::string name, std::string value) {
        if(name == "font") {
            fonts[c_font] = sf::Font();
            fonts[c_font].loadFromFile(value);
            w->setProperty(name, sui::Property::make(&fonts[c_font]));
            c_font++;
        } else if(name == "outlineThickness" || name == "fontSize" || name == "textPadding" || name == "padding") {
            w->setProperty(name, sui::Property::make<float>(std::stof(value)));
        } else if(name == "fillColor" || name == "fillColorHovered" || name == "fillColorClicked" ||
                  name == "textColor" || name == "outlineColor" || name == "normalColor" || name == "errorColor") {
            sf::Color color;
            unsigned int i = 0;
            auto getV = [&i, &value](){
                std::string v = "";
                for(;i < value.size(); i++) {
                    char c = value[i];
                    if(c == ',') {
                        i++;
                        break;
                    }
                    v += c;
                }
                return v;
            };
            color.r = std::stof(getV());
            color.g = std::stof(getV());
            color.b = std::stof(getV());
            color.a = std::stof(getV());
            w->setProperty(name, sui::Property::make(color));
        } else if(name == "textAlignX" || name == "textAlignY") {
            sui::ORIGIN o;
            if(value == "start") { o = sui::ORIGIN_START;}
            else if(value == "end") {o = sui::ORIGIN_END;}
            else {o = sui::ORIGIN_MIDDLE;}
            w->setProperty(name, sui::Property::make(o));
        }
    }
    ~ThemeReader() {
        file.close();
    }
    sf::Font fonts[10]; // just in case :)
    int c_font = 0;
    std::ifstream file;
};

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
    check_box->setProperty("outlineThickness", sui::Property::make(2.f));
    // 25 is calculated for lack of a better method
    check_box->setProperty("boxSize", sui::Property::make(sf::Vector2f(25, 25)));
    check_box->setProperty("scaleTypeX", BoxAbs);
    check_box->setProperty("scaleTypeY", BoxAbs);
    
    sui::FrameLayout *auto_sort_text_frame = static_cast<sui::FrameLayout *>(hbox->addChild(new sui::FrameLayout()));
    auto *auto_sort_text = auto_sort_text_frame->addChild(new sui::Text());
    reader.setWidgetProps(auto_sort_text, "autoSortText");
    auto_sort_text->setProperty("text", sui::Property::make<sf::String>("Auto Sort"));
    auto_sort_text_frame->setProperty("frameSize", sui::Property::make(sf::Vector2f(1,.5)));
    auto_sort_text_frame->setProperty("framePos", sui::Property::make(sf::Vector2f(0,.5)));
    
    auto *divider = vbox->addChild(new sui::ColorBox());
    reader.setWidgetProps(divider, "divider");
    divider->setProperty("boxSize", sui::Property::make(sf::Vector2f(1, 3)));
    divider->setProperty("scaleTypeY", BoxAbs);
    
    
    
    TableLayout *table = new TableLayout(
        [&reader](sui::Widget *text){
            reader.setWidgetProps(text, "tableText");
        },
        [&reader](sui::Widget *addButton){
            reader.setWidgetProps(addButton, "addButton");
        });
    reader.setWidgetProps(table, "table");
    table->setProperty("onAdded", sui::Property::makeFunc([table, vbox](){
        vbox->setSize(sf::Vector2f(table->getUsedWidth(),138));
        vbox->update();
    }));
    
    table->addedTextField = [&reader](sui::Widget *textfield){
        reader.setWidgetProps(textfield, "tableTextField");
    };
    vbox->setSize(sf::Vector2f(table->getUsedWidth(),138));
    table->setProperty("boxSize", sui::Property::make(sf::Vector2f(1, 100)));
    table->setProperty("scaleTypeX", sui::Property::make(sui::BoxLayout::RELATIVE));
    table->setProperty("scaleTypeY", sui::Property::make(sui::BoxLayout::ABSOLUTE));
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
