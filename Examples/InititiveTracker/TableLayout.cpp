#include "TableLayout.hpp"
#include <algorithm>
#include <string>

BoxLayout::BoxLayout() : sui::BoxLayout() {
    ;
}
void BoxLayout::onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
    sui::BoxLayout::onDraw(target, states);
    for(auto &l : lines) {
        target.draw(l, states);
    }
}

void BoxLayout::setLineColors(sf::Color color) {
    mColor = color;
    for(auto &l : lines) {
        l.setFillColor(color);
    }
}

void BoxLayout::onUpdate() {
    sui::BoxLayout::onUpdate();
    lines.clear();
    // items flow horizontally so lines are vertical between them
    if(getDirection() == HORIZONTAL) {
        // place bar after every item except the last one
        for(unsigned int i = 0; i < getChildren().size()-1; i++) {
            auto &c = getChildren()[i];
            const float place = c->getGlobalBounds().left + c->getSize().x;
            const float width = getPadding();
            lines.push_back(sf::RectangleShape(sf::Vector2f(width, getSize().y)));
            lines.back().setPosition(sf::Vector2f(place, getGlobalBounds().top));
            
        }
    } else {
        // place bar after every item except the last one
        for(unsigned int i = 0; i < getChildren().size()-1; i++) {
            auto &c = getChildren()[i];
            const float place = c->getGlobalBounds().top + c->getSize().y;
            const float height = getPadding();
            lines.push_back(sf::RectangleShape(sf::Vector2f(getSize().x, height)));
            lines.back().setPosition(sf::Vector2f(getGlobalBounds().left, place));
        }
    }
    setLineColors(getProperty("outlineColor").as<sf::Color>());
}





TableLayout::TableLayout() : BoxLayout() {
    setProperty("direction", sui::Property::make(sui::BoxLayout::HORIZONTAL));
    BoxLayout *vbox = new BoxLayout();
    vbox->setProperty("direction", sui::Property::make(sui::BoxLayout::VERTICAL));
    vbox->setProperty("boxSize", sui::Property::make(sf::Vector2f(text_width,1)));
    vbox->setProperty("scaleTypeX", sui::Property::make(sui::BoxLayout::ABSOLUTE));
    vbox->setProperty("scaleTypeY", sui::Property::make(sui::BoxLayout::RELATIVE));
    addChild(vbox);
    
    auto addText = [&vbox](sf::String &&str) {
        sui::FrameLayout *frame = new sui::FrameLayout();
        sui::Text *text = new sui::Text();
        text->setProperty("text", sui::Property::make<sf::String>(std::move(str)));
        text->setProperty("frameSize", sui::Property::make(sf::Vector2f(1,.6)));
        text->setProperty("framePos", sui::Property::make(sf::Vector2f(0,.5)));
        text->setProperty("scaleTypeX", sui::Property::make(sui::BoxLayout::RELATIVE));
        text->setProperty("scaleTypeY", sui::Property::make(sui::BoxLayout::RELATIVE));
        text->setProperty("posTypeX", sui::Property::make(sui::BoxLayout::RELATIVE));
        text->setProperty("posTypeY", sui::Property::make(sui::BoxLayout::RELATIVE));
        // text->setProperty("textColor", sui::Property::make(sf::Color(0,0,0,255)));
        
        frame->addChild(text);
        vbox->addChild(frame);
    };
    
    addText("Name");
    addText("Inititive Roll");
    addText("Inititive Mod");
    addText("Health");
    
    
    add_button = new sui::Button();
    addChild(add_button);
    add_button->setProperty("text", sui::Property::make<sf::String>("+"));
    add_button->setProperty("fontSize", sui::Property::make(36.f));
    add_button->setProperty("textAlignX", sui::Property::make(sui::ORIGIN_MIDDLE));
    add_button->setProperty("textAlignY", sui::Property::make(sui::ORIGIN_MIDDLE));
    add_button->setProperty("outlineThickness", sui::Property::make(0.f));
    add_button->setProperty("boxSize", sui::Property::make(sf::Vector2f(button_width, 1)));
    add_button->setProperty("scaleTypeX", sui::Property::make(sui::BoxLayout::ABSOLUTE));
    add_button->setProperty("scaleTypeY", sui::Property::make(sui::BoxLayout::RELATIVE));
    add_button->setProperty("onClickedDown", sui::Property::makeFunc([this](){
        addColumn();
        onAdded();
    }));
    
    
    shouldAutoSort = false;
    
    setProperty("outlineColor", sui::Property::make(sf::Color(0,0,0,255)));
}

void TableLayout::addColumn() {
    BoxLayout *vbox = new BoxLayout();
    insertChild(vbox, getChildren().size()-1);
    vbox->setProperty("scaleTypeY", sui::Property::make(sui::BoxLayout::RELATIVE));
    vbox->setProperty("boxScale", sui::Property::make(sf::Vector2f(column_width,1)));
    vbox->setProperty("direction", sui::Property::make(sui::BoxLayout::VERTICAL));
    
    for(unsigned int i = 0;i < 4; i++) {
        sui::TextField *textfield = new sui::TextField();
        textfield->setProperty("outlineThickness", sui::Property::make(0.f));
        if(i == 1 || i == 2)
            textfield->setProperty("onChanged", sui::Property::makeFunc([this](){
                autoSort();
            }));
        vbox->addChild(textfield);
    }
}
void TableLayout::autoSort() {
    if(shouldAutoSort) {
        sort();
    }
}
void TableLayout::sort() {
    std::sort(mChildren.begin()+1, mChildren.end()-1,
        [](sui::Widget *i, sui::Widget *j){
            const sui::Widget *text_field_roll_i = static_cast<sui::BoxLayout *>(i)->getChildren()[1];
            const sui::Widget *text_field_mod_i = static_cast<sui::BoxLayout *>(i)->getChildren()[2];
            const std::string roll_string_i = text_field_roll_i->getProperty("text").as<sf::String>().toAnsiString();
            const std::string mod_string_i = text_field_mod_i->getProperty("text").as<sf::String>().toAnsiString();
            int roll_i;
            int mod_i;
            try {
                mod_i = std::stoi(mod_string_i);
            } catch(std::invalid_argument arg) {
                mod_i = 0;
            }
            try {
                roll_i = std::stoi(roll_string_i)+mod_i;
            } catch(std::invalid_argument arg) {
                return false;
            }
            
            const sui::Widget *text_field_roll_j = static_cast<sui::BoxLayout *>(j)->getChildren()[1];
            const sui::Widget *text_field_mod_j = static_cast<sui::BoxLayout *>(j)->getChildren()[2];
            const std::string roll_string_j = text_field_roll_j->getProperty("text").as<sf::String>().toAnsiString();
            const std::string mod_string_j = text_field_mod_j->getProperty("text").as<sf::String>().toAnsiString();
            int roll_j;
            int mod_j;
            try {
                mod_j = std::stoi(mod_string_j);
            } catch(std::invalid_argument arg) {
                mod_j = 0;
            }
            try {
                roll_j = std::stoi(roll_string_j)+mod_j;
            } catch(std::invalid_argument arg) {
                return false;
            }
            
            if(roll_i > roll_j) {
                return true;
            } else if(roll_i < roll_j) {
                return false;
            } else {
                if(mod_i > mod_j) {
                    return true;
                } else if(mod_i < mod_j) {
                    return false;
                } else {
                    return false; //keep order if tied cause why move?
                }
            }
        });
    update();
}

float TableLayout::getUsedWidth() {
    return (getChildren().size()-2)*column_width + button_width + text_width;
}


